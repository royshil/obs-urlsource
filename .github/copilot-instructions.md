# Copilot Instructions for obs-urlsource

## Project Overview

**obs-urlsource** is a plugin for [OBS Studio](https://obsproject.com) that enables fetching data from URLs, API endpoints, or files and displaying them as text, images, or audio in OBS scenes.

- **Primary Language**: C/C++ (C++17 standard)
- **License**: GNU General Public License v2
- **Platforms**: Windows, macOS (Intel & Apple Silicon), Linux (Ubuntu)
- **Build System**: CMake 3.16+ with custom build scripts

## Core Functionality

The plugin provides:
- HTTP request support (GET, POST) with custom headers and body
- Multiple output parsing methods: JSON (JSONPointer & JSONPath), XML/HTML (XPath & XQuery), Key-Value, Regex, CSS selectors
- Dynamic templating using [Inja](https://github.com/pantor/inja) template engine
- WebSocket support for real-time data
- Output routing to multiple OBS sources
- Image rendering from URLs or base64 data
- HTML4/CSS subset rendering for rich text display
- Audio output to external media sources

## Repository Structure

```
obs-urlsource/
├── src/                    # Source code
│   ├── parsers/           # Data parsing implementations
│   ├── ui/                # UI components and dialogs
│   ├── plugin-main.c      # Plugin entry point
│   ├── url-source.cpp     # Main source implementation
│   ├── request-data.cpp   # HTTP request handling
│   ├── websocket-client.cpp # WebSocket implementation
│   └── ...
├── vendor/                # Third-party dependencies (submodules)
│   ├── inja/             # Template engine
│   ├── jsoncons/         # JSON parsing
│   └── nlohmann-json/    # JSON library
├── cmake/                 # CMake modules and helpers
├── build-aux/            # Build auxiliary files
├── data/                 # Plugin resources (localization, etc.)
├── .github/              # GitHub workflows and actions
├── CMakeLists.txt        # Root CMake configuration
└── buildspec.json        # Build specification and dependencies
```

## Build System

### Dependencies Management
- **OBS Studio**: Version 31.1.1 (specified in buildspec.json)
- **Submodules**: Must be initialized before building (`git submodule update --init --recursive`)
- **System Dependencies**: cURL (can use system or bundled), pugixml

### Build Commands

**macOS:**
```bash
./.github/scripts/build-macos -c Release
./.github/scripts/package-macos -c Release  # For installer
```

**Linux (Ubuntu):**
```bash
./.github/scripts/build-ubuntu
```

**Windows:**
```powershell
.github/scripts/Build-Windows.ps1 -Target x64 -CMakeGenerator "Visual Studio 17 2022"
```

### CMake Options
- `ENABLE_FRONTEND_API`: Use obs-frontend-api (default: OFF)
- `ENABLE_QT`: Use Qt functionality (default: OFF)
- `USE_SYSTEM_CURL`: Use system cURL instead of bundled (default: OFF)
- `USE_SYSTEM_PUGIXML`: Use system pugixml (default: OFF)

## Code Style and Formatting

### C/C++ Code
- **Formatter**: clang-format version 16+
- **Standard**: C++17
- **Indentation**: Tabs (width 8) for continuation and indentation
- **Column Limit**: 100 characters
- **Pointer Alignment**: Right (`int *ptr`)
- **Brace Style**: Custom (functions on new line, control statements on same line)
- **Header Comments**: All source files include GPL v2 license header

### CMake Files
- **Formatter**: gersemi
- **Line Length**: 120 characters
- **Indentation**: 2 spaces
- **List Expansion**: favour-inlining

### Checking Format
```bash
# Automatic format check runs in CI for master/main branches
# See .github/workflows/check-format.yaml
```

## Development Guidelines

### Code Organization
1. **Plugin Entry**: `src/plugin-main.c` - OBS module initialization
2. **Source Implementation**: `src/url-source.cpp` - Main source logic
3. **Request Handling**: `src/request-data.cpp` - HTTP/WebSocket operations
4. **Parsing**: `src/parsers/` - Various data format parsers
5. **UI Components**: `src/ui/` - Configuration dialogs and helpers

### Key Data Structures
- `url_source_data`: Main plugin data structure (defined in `url-source-data.h`)
- `request_data_handler_response`: HTTP response handling
- `mapping_data`: Output mapping to OBS sources

### Threading
- The plugin uses threaded operations for network requests
- Mutex protection for shared data (see `curl_mutex`, `output_mapping_mutex`)
- Thread control via condition variables

### Template System
- Uses Inja template engine for dynamic content
- Input templates support `{{input}}` variable and `strftime`, `urlencode` functions
- Output templates support `{{output}}`, `{{output1}}`, `{{output2}}`, etc., and `{{body}}`

## Testing

**Note**: This project does not currently have formal unit tests or test infrastructure. Changes should be manually tested with OBS Studio.

### Manual Testing Workflow
1. Build the plugin for your platform
2. Copy to OBS plugins directory:
   - **macOS**: `~/Library/Application Support/obs-studio/plugins`
   - **Linux**: `/usr/lib/x86_64-linux-gnu/` and `/usr/share/`
   - **Windows**: OBS installation directory
3. Launch OBS Studio and add URL/API Source
4. Test various scenarios (GET/POST requests, different parsers, output formats)

## CI/CD Workflow

### Pull Requests
- Format checking (clang-format, gersemi) runs automatically
- Builds for all platforms
- No packaging unless "Seeking Testers" label is applied

### Push to main/master
- Format checking
- Full build for all platforms
- Packages created but not released

### Tagged Releases
- Versions follow semver: `X.Y.Z` or `X.Y.Z-beta/rc`
- Automated release creation with installers/packages
- Draft release created for review before publishing

## Common Patterns

### Adding New Parsers
1. Create parser in `src/parsers/`
2. Follow existing parser structure (see `parsers/` directory)
3. Register in request data handler
4. Add UI controls if needed

### Adding UI Components
1. UI code lives in `src/ui/`
2. Follow Qt patterns if Qt is enabled
3. Match existing dialog style

### Memory Management
- Use OBS memory functions: `bmalloc`, `bfree`, `bzalloc`
- RAII patterns for C++ resources
- Careful cleanup in destroy callbacks

## External Resources

- [OBS Studio Plugin Development](https://obsproject.com/docs/)
- [Inja Template Documentation](https://github.com/pantor/inja)
- [Project README](../README.md) - User documentation and tutorials
- [Public APIs List](https://github.com/public-apis/public-apis) - API testing resources

## Licensing Notes

- All code must be GPL v2 compatible
- Include GPL v2 header in new source files
- Copyright: Roy Shilkrot (roy.shil@gmail.com)
- Vendor libraries have their own licenses (check vendor/ subdirectories)

## Getting Help

- Issues: GitHub Issues
- Code walkthrough: https://youtu.be/TiluUg1LxcQ
- Sponsor: https://github.com/sponsors/royshil
