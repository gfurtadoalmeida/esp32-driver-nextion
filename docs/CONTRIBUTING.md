# Contributing

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways to help and details about how this project handles them. Please make sure to read the relevant section before making your contribution. It will make it a lot easier for us maintainers and smooth out the experience for all involved.

> And if you like the project, but just don't have time to contribute, that's fine. There are other easy ways to support the project and show your appreciation, which we would also be very happy about:
>
> * Star the project.
> * Tweet about it.
> * Refer this project in your project's readme.
> * Mention the project at local meetups and tell your friends/colleagues.

## Table of Contents

- [Contributing](#contributing)
  - [Table of Contents](#table-of-contents)
  - [Code of Conduct](#code-of-conduct)
  - [I Have a Question](#i-have-a-question)
  - [I Want To Contribute](#i-want-to-contribute)
    - [Reporting Bugs](#reporting-bugs)
      - [Before Submitting a Bug Report](#before-submitting-a-bug-report)
      - [How Do I Submit a Good Bug Report?](#how-do-i-submit-a-good-bug-report)
    - [Suggesting Enhancements](#suggesting-enhancements)
      - [Before Submitting an Enhancement](#before-submitting-an-enhancement)
      - [How Do I Submit a Good Enhancement Suggestion?](#how-do-i-submit-a-good-enhancement-suggestion)
  - [Development Guidelines](#development-guidelines)
  - [Requirements](#requirements)
  - [Attribution](#attribution)

## Code of Conduct

This project and everyone participating in it is governed by the [Code of Conduct](/docs/CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code.

## I Have a Question

Before you ask a question, it is best to search for existing [Issues][issues-page] that might help you. In case you have found a suitable issue and still need clarification, you can write your question in this issue.

If you then still feel the need to ask a question and need clarification, we recommend the following:

* Open an [Issue][issues-new].
* Provide as much context as you can about what you're running into.

## I Want To Contribute

> ### Legal Notice
>
> When contributing to this project, you must agree that you have authored 100% of the content, that you have the necessary rights to the content and that the content you contribute may be provided under the project license.

### Reporting Bugs

#### Before Submitting a Bug Report

A good bug report shouldn't leave others needing to chase you up for more information. Therefore, we ask you to investigate carefully, collect information and describe the issue in detail in your report. Please complete the following steps in advance to help us fix any potential bug as fast as possible.

* Make sure that you are using the latest version.
* Determine if your bug is really a bug and not an error on your side e.g. using incompatible environment components/versions.
* To see if other users have experienced (and potentially already solved) the same issue you are having, check if there is not already a bug report existing for your bug or error in the [bug tracker][issues-bug].

#### How Do I Submit a Good Bug Report?

> You must never report security related issues, vulnerabilities or bugs including sensitive information to the issue tracker, or elsewhere in public. Instead sensitive bugs must be sent to [@gfurtadoalmeida][twitter].

We use GitHub issues to track bugs and errors. If you run into an issue with the project:

* Open an [issue][issues-new]. (Since we can't be sure at this point whether it is a bug or not, we ask you not to talk about a bug yet and not to label the issue.)
* Explain the behavior you would expect and the actual behavior.
* Please provide as much context as possible and describe the *reproduction steps* that someone else can follow to recreate the issue on their own. This usually includes your code. For good bug reports you should isolate the problem and create a reduced test case.

Once it's filed:

* The project team will label the issue accordingly.
* A team member will try to reproduce the issue with your provided steps. If there are no reproduction steps or no obvious way to reproduce the issue, the team will ask you for those steps and mark the issue as `needs-repro`. Bugs with the `needs-repro` tag will not be addressed until they are reproduced.
* If the team is able to reproduce the issue, it will be marked `needs-fix`, as well as possibly other tags (such as `critical`), and the issue will be left to be implemented by someone.

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion, **including completely new features and minor improvements to existing functionality**. Following these guidelines will help maintainers and the community to understand your suggestion and find related suggestions.

#### Before Submitting an Enhancement

* Make sure that you are using the latest version.
* Perform a [search][issues-page] to see if the enhancement has already been suggested. If it has, add a comment to the existing issue instead of opening a new one.
* Find out whether your idea fits with the scope and aims of the project. It's up to you to make a strong case to convince the project's developers of the merits of this feature. Keep in mind that we want features that will be useful to the majority of our users and not just a small subset. If you're just targeting a minority of users, consider writing an add-on/plugin library.

#### How Do I Submit a Good Enhancement Suggestion?

Enhancement suggestions are tracked as [GitHub issues][issues-page].

* Use a **clear and descriptive title** for the issue to identify the suggestion.
* Provide a **step-by-step description of the suggested enhancement** in as many details as possible.
* **Describe the current behavior** and **explain which behavior you expected to see instead** and why. At this point you can also tell which alternatives do not work for you.
* You may want to **include screenshots and animated GIFs** which help you demonstrate the steps or point out the part which the suggestion is related to.
* **Explain why this enhancement would be useful** to most users. You may also want to point out the other projects that solved it better and which could serve as inspiration.

## Development Guidelines

* Branch from the head of the default branch.
* Follow the [ESP-IDF code conventions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/contribute/style-guide.html).
* The code must support any [ESP32 module](https://www.espressif.com/en/products/modules).
* Use portable types:
  * ```stdbool.h```: [bool](https://en.wikipedia.org/wiki/C_data_types#Boolean_type).
  * ```stdint.h```: [fixed-width integer](https://en.wikipedia.org/wiki/C_data_types#stdint.h).
  * ```stddef.h```: [pointers and sizes](https://en.wikipedia.org/wiki/C_data_types#Size_and_pointer_difference_types).
* Do not hide pointers behind typedefs.
* Create tests for every new functionality.
* Examples:
  * Must have an HMI file.
  * HMI must use the basic display version unless an advanced feature is needed.

## Requirements

* [![ESP-IDF][esp-idf-badge]][esp-idf-url]
* [![Docker][docker-badge]][docker-url] (only if building using the provided script).

## Attribution

This guide is based on the **contributing.md**. [Make your own](https://contributing.md/)!

[docker-badge]: https://img.shields.io/badge/docker-2496ED?logo=docker&style=for-the-badge&logoColor=FFFFFF
[docker-url]: https://www.docker.com/
[esp-idf-badge]: https://img.shields.io/badge/espressif-5.3-E7352C?logo=espressif&style=for-the-badge
[esp-idf-url]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html
[issues-bug]: https://github.com/gfurtadoalmeida/esp32-driver-nextion/labels/bug
[issues-page]: https://github.com/gfurtadoalmeida/esp32-driver-nextion/issues
[issues-new]: https://github.com/gfurtadoalmeida/esp32-driver-nextion/issues/new/choose
[twitter]: https://x.com/gfurtadoalmeida
