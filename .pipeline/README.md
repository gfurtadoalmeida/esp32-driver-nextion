# Pipeline for Build Automation

The pipeline was made for [Azure DevOps](https://azure.microsoft.com/en-us/services/devops/).

On your Azure DevOps, the agent must have the following [custom capability](https://docs.microsoft.com/en-us/azure/devops/pipelines/process/demands?view=azure-devops&tabs=yaml):

* `ESP-IDF` = `True`

In the [virtual-machine](/.pipeline/virtual-machine/) folder there are scripts and ARM templates to create a virtual machine capable of building ESP-IDF projects.

## Virtual Machine Characteristics

* Infrastructure: [Azure](https://azure.microsoft.com/en-us/)
* OS: Windows 10
* Software installed:
  * [ESP-IDF Framework for Windows](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html)
  * [ESP-IDF sources](https://github.com/espressif/esp-idf) (latest release version).
  * [Microsoft Visual Studio Enterprise 2019](https://visualstudio.microsoft.com/vs/enterprise/) (for the MSVC compiler)
  * [Azure DevOps Pipeline Agent](https://github.com/microsoft/azure-pipelines-agent) (so the VM can communicate with Azure DevOps).

## Deploying the Infrastructure

Just click the button bellow and follow the instructions.

[![Deploy to Azure](https://aka.ms/deploytoazurebutton)](https://portal.azure.com/#create/Microsoft.Template/uri/https%3A%2F%2Fraw.githubusercontent.com%2Fgfurtadoalmeida%2Fesp32-project-template%2Fmaster%2F.pipeline%2Fvirtual-machine%2Ftemplate.json)

## Installing the Software

### Requirements

* Azure DevOps [Personal Access Token (PAT)](https://docs.microsoft.com/en-us/azure/devops/organizations/accounts/use-personal-access-tokens-to-authenticate?view=azure-devops&tabs=preview-page).
* Azure DevOps [organization URL](https://docs.microsoft.com/en-us/azure/devops/extend/develop/work-with-urls?view=azure-devops&tabs=http).

### Procedure

1. Open a RDP session to the VM.
2. Copy [Install-Tools.ps1](/.pipeline/virtual-machine/Install-Tools.ps1)
3. Edit the script, adding the missing values:
   * Line 1: organization URL.
   * Line 2: Personal Access Token (PAT).
4. Open PowerShell as Administrator:
   1. Run `Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope CurrentUser`
   2. Run the script.
   3. Run `Set-ExecutionPolicy -ExecutionPolicy AllSigned -Scope CurrentUser`

Unfortunately the ESP-IDF installer opens a command prompt at the end of the installation, you need to close it so the script can proceed.
