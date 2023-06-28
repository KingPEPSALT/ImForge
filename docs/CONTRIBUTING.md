# Contributing
Anyone can contribute by making pull requests which will be reviewed by GitHub collaborators. Note that currently, when compiling for Windows, [<u>GCC</u>](https://gcc.gnu.org/) has been known to fail, so use [<u>MSVC</u>](https://visualstudio.microsoft.com/vs/features/cplusplus/) when possible. 

## Cloning and Building
We clone like so, using `--recursive` to also clone the submodules in [__external/__](../external/):
```bash
git clone --recursive https://github.com/KingPEPSALT/ImForge.git
```
We compile and build with [<u>CMake</u>](https://cmake.org/), specifically versions 3.25.0+:
```bash
cd ImForge
cmake -S . -B _build 
cmake --build _build --config debug
```
It is recommended that you use [<u>VSCode</u>](https://code.visualstudio.com/) with the [<u>C/C++ Extension Pack</u>](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) (which comes with the useful CMake extensions); and for documentation writers, the [<u>Markdown All in One</u>](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one) extension comes in handy. Though these are only personal preference.

---
## Commit Practice
Stick to using [<u>conventional commits</u>](https://www.conventionalcommits.org/en/v1.0.0/) whilst developing within this repo. This will make it easier for not only contributors to read through commits when necessary (and use related tools) but also helps interested users understand the state of the repository quickly.

Never commit your CMake build directory (which usually defaults to `./build/`); feel free to put your build directory in the [__.gitignore__](../.gitignore) file:
```sh
# CMake build directories
_build
build
...
your_build_directory_here
```

---
## Technical Details
The libraries we use are listed in the [__README.md__](./README.md). Specifically, we use [<u>SDL3</u>](https://github.com/libsdl-org/sdl) with the SDL3Renderer for OS-agnostic rendering (as it will select the correct render driver for us). SDL3 is (as of 2023-06-03), quite new and may have issues, though these should be ultimately sparse as we won't be using SDL directly all too much. 

---
## Document Writers

> :information_source: When creating an issue, as a developer, it would be wise to follow this styling outline to ensure readability and consistency. Though, as you are a developer, your input and discretion is important so this is not a concrete rule.

For those who don't mind changing the [__README.md__](./README.md) here and there when necessary, please note that, as there are duplicate files the [__root__](..) directory and in [__this__](.) directory, you should be changing both of these and adjusting the relative links so they resolve correctly. We also stick to a style with links as detailed below.

### Links
Internal reference links, such as [__docs/CONTRIBUTIG.md__](./CONTRIBUTING.md) are bold - and we tend to prefer to write out the path (from the repository root) and file extension (as far as is reasonable);
```md
[__docs/CONTRIBUTING.md__](./CONTRIBUTING.md)
```
> links to folders should end in a slash like so: [__include/core/__](../include/core)

and external links, such as [<u>this repository</u>](https://github.com/KingPEPSALT/ImForge) are underlined:
```md
[<u>this repository</u>](https://github.com/KingPEPSALT/ImForge)
```

Please use the present documentation as a reference when unsure.

