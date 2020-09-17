$vsts_agent_server_url = ''
$vsts_agent_auth_pat = ''
$vsts_agent_pool = 'Default'
$vsts_user_account = 'NT AUTHORITY\Network Service'

Set-Location \

$ErrorActionPreference = "Stop"

Write-Host "Installing tools..."

function Find-Url {
    param (
        [Parameter(Mandatory = $true)]
        [ValidateNotNullOrEmpty()]
        [string]
        $Url,
        [Parameter(Mandatory = $true)]
        [ValidateNotNullOrEmpty()]
        [string]
        $Pattern,
        [Parameter(Mandatory = $true)]
        [ValidateNotNullOrEmpty()]
        [string]
        $Format
    )

    $body = Invoke-RestMethod -Uri $Url -Method Get

    if (($body -match $Pattern) -eq $false) {
        throw "No match for url: '$Url'"
    }

    return $ExecutionContext.InvokeCommand.ExpandString($Format)
}

function Download-File {
    param(
        [Parameter(Mandatory = $true)]
        [ValidateNotNullOrEmpty()]
        [string]
        $Name,

        [Parameter(Mandatory = $true)]
        [ValidateNotNullOrEmpty()]
        [string]
        $Uri,

        [Parameter(Mandatory = $true)]
        [ValidateNotNullOrEmpty()]
        [string]
        $Destination
    )

    Write-Host "Downloading $Name..."

    if (!(Test-Path -Path $Destination -PathType Leaf)) {
        $wc = New-Object System.Net.WebClient
        $wc.DownloadFile($Uri, $Destination)
    }

    Write-Verbose 'done.'
}

# Download the ESP-IDF Framework tools.

$esp_idf_download_path = "$env:TEMP\espidf.exe"
$esp_idf_install_dir = 'C:\espidf'

Download-File -Name 'espidf' `
    -Uri 'https://dl.espressif.com/dl/esp-idf-tools-setup-2.3.exe' `
    -Destination $esp_idf_download_path

# ESP-IDF installer will install on whatever folder
# is set on env var 'IDF_TOOLS_PATH'.
# We want to install it on 'C:\espidf'.

$env:IDF_TOOLS_PATH = $esp_idf_install_dir

Write-Host 'Installing ESP-IDF Framework Tools...'

Start-Process $esp_idf_download_path `
    -ArgumentList @('/SILENT', '/ALLUSERS', '/CLOSEAPPLICATIONS', '/SUPPRESSMSGBOXES', '/SP-') `
    -Wait

# The installation folder must be set on machine env var PATH
# too so the agent can read it.
[System.Environment]::SetEnvironmentVariable('IDF_TOOLS_PATH', $esp_idf_install_dir, [System.EnvironmentVariableTarget]::Machine)

Write-Host 'done.'

# The installer will download the ESP-IDF Framework sources
# on '{current user}/desktop/esp-idf'.
# We need them on 'C:\espidfsources', so the build can access it.

$esp_idf_old_source_dir = "$([System.Environment]::GetFolderPath([System.Environment+SpecialFolder]::Desktop))\esp-idf\"

Write-Host 'Moving ESP-IDF source folder...'

Rename-Item -Path $esp_idf_old_source_dir -NewName 'espidfsources'
Move-Item -Path "$([System.Environment]::GetFolderPath([System.Environment+SpecialFolder]::Desktop))\espidfsources" -Destination 'C:\'

Write-Host 'done.'

# Set access rules for ESP-IDF folders.

Write-Host 'Setting access rules...'

$access_rule = New-Object System.Security.AccessControl.FileSystemAccessRule($vsts_user_account, "FullControl", "ContainerInherit,ObjectInherit", "None", "Allow")

$acl = Get-Acl 'C:\espidf'
$acl.SetAccessRule($access_rule)
$acl | Set-Acl 'C:\espidf'

$acl = Get-Acl 'C:\espidfsources'
$acl.SetAccessRule($access_rule)
$acl | Set-Acl 'C:\espidfsources'

Write-Host 'done.'

# Add Python to machine env var 'PATH' so the agent can recognize "python.exe".
# The VM image used (Visual Studio Professional 20XX on Windows 10)
# already have Python installed, it's just not on the env var.

$sys_path = [System.Environment]::GetEnvironmentVariable('PATH', [System.EnvironmentVariableTarget]::Machine)

[System.Environment]::SetEnvironmentVariable('PATH', "$sys_path;C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python37_64", [System.EnvironmentVariableTarget]::Machine)

# Download, unzip and install the VSTS Pipeline Agent
# on 'C:\vstsagent' folder.

$vsts_agent_url = Find-Url -Url 'https://github.com/microsoft/azure-pipelines-agent/releases' `
    -Pattern 'agent\/([\.\d]+)\/([A-z-]*-win-x64-[\.\d]+\.zip)' `
    -Format 'https://vstsagentpackage.azureedge.net/agent/$($Matches[1])/$($Matches[2])'

$vsts_agent_unzip_dir = New-Item -Path 'C:\' -Name 'vstsagent' -ItemType Directory
$vsts_agent_download_path = "$env:TEMP\vstsagent.zip"

Download-File -Name 'vstsagent' -Uri $vsts_agent_url -Destination $vsts_agent_download_path
Expand-Archive -Path $vsts_agent_download_path -DestinationPath $vsts_agent_unzip_dir

Write-Host 'Installing Azure DevOps Agent...'

Start-Process "$vsts_agent_unzip_dir\config.cmd" `
    -ArgumentList @(
    '--unattended',
    '--acceptTeeEula',
    '--runAsService',
    '--runAsAutoLogon',
    '--auth pat',
    "--url $vsts_agent_server_url",
    "--token $vsts_agent_auth_pat",
    "--pool $vsts_agent_pool",
    "--windowsLogonAccount '$vsts_user_account'"
) `
    -Wait

Write-Host 'done.'

Write-Host "Tools installation done!"