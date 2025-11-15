# Filter Output Feature - Usage Guide

## Overview

The URL/API Source plugin now supports outputting data directly to OBS source filters. This allows you to dynamically control filter parameters (like Color Correction's color multiply, brightness, contrast, etc.) based on API responses or fetched data.

## Use Case Example

A common use case is updating a Color Correction filter's "Color Multiply" setting based on data from Google Sheets or another API. For example, in an esports event, you could:
- Fetch team colors from a spreadsheet
- Dynamically update overlay colors based on the current match
- Control visual effects parameters from external data sources

## How to Use

### 1. Setup Your Filter

First, add a filter to your source in OBS:
1. Right-click on your source in OBS
2. Select "Filters"
3. Add the filter you want to control (e.g., "Color Correction")
4. Note the exact name of the filter

### 2. Configure URL Source

1. Add or edit a URL/API Source
2. Configure your URL/API request to fetch the data
3. Set up the output parsing (JSON, XPath, etc.) to extract the value you want

### 3. Configure Output Mapping

1. Click "Output Mapping" button in the URL Source settings
2. Click "+" to add a new mapping
3. In the "Output" dropdown, you'll now see filters listed as:
   ```
   (Type) SourceName -> FilterName
   ```
   For example:
   ```
   (Text) MyOverlay -> Color Correction
   ```

4. Select the filter you want to control

5. In the "Filter Setting Name" field, enter the exact property name you want to update.
   Common examples for Color Correction filter:
   - `multiply` - Color Multiply (ARGB hex value like 0xFFRRGGBB)
   - `add` - Color Add
   - `gamma` - Gamma
   - `contrast` - Contrast
   - `brightness` - Brightness
   - `saturation` - Saturation
   - `hue_shift` - Hue Shift
   - `opacity` - Opacity

6. Configure your output template (e.g., `{{output}}`) to format the API response appropriately

### 4. Value Types

The plugin automatically detects and converts value types:
- **Integer**: Whole numbers (e.g., `255`, `1`, `0`)
- **Double**: Decimal numbers (e.g., `1.5`, `0.75`)
- **String**: Text values (e.g., `"#FF0000"`, `"enabled"`)

For Color Correction's multiply setting, you typically want to send an integer representing the ARGB color:
- Format: `0xFFRRGGBB` or as decimal
- Example: `0xFFFF0000` (opaque red) = `4294901760` in decimal

### 5. Template Example

For a Google Sheets API response that returns a color value:

**API Response:**
```json
{
  "values": [
    ["4294901760"]
  ]
}
```

**JSON Path:** `$.values[0][0]`

**Template:** `{{output}}`

**Filter Setting Name:** `multiply`

This will update the Color Correction filter's color multiply setting to red.

## Finding Filter Setting Names

To find the exact setting name for a filter:

1. **Check OBS Source Code**: Look in the OBS Studio repository for the filter's source code
2. **Use OBS Logs**: Enable logging and watch for property names when manually adjusting the filter
3. **Common Filters**:
   - **Color Correction**: multiply, add, gamma, contrast, brightness, saturation, hue_shift, opacity
   - **Color Key**: key_color, similarity, smoothness
   - **Chroma Key**: key_color_type, similarity, smoothness, brightness
   - **Scaling/Aspect Ratio**: resolution
   - **Crop/Pad**: left, top, right, bottom

## Advanced Examples

### Example 1: Team Color from Google Sheets

**Scenario**: Update scoreboard color based on current team

**Google Sheets API Response:**
```json
{
  "values": [
    ["Team A", "4294901760"]
  ]
}
```

**Configuration:**
- JSON Path: `$.values[0][1]`
- Output: `(Text) Scoreboard -> Color Correction`
- Filter Setting: `multiply`
- Template: `{{output}}`

### Example 2: Multiple Properties

You can create multiple mappings to control different properties:

**Mapping 1:**
- Output: `MySource -> Color Correction`
- Setting: `multiply`
- Template: `{{color}}`

**Mapping 2:**
- Output: `MySource -> Color Correction`
- Setting: `opacity`
- Template: `{{opacity}}`

## Troubleshooting

### Filter Not Showing Up
- Make sure the filter exists on the source before configuring the mapping
- Verify the source has filters by checking in OBS Filters dialog
- Refresh the URL Source dialog to reload the filter list

### Value Not Updating
- Check the OBS logs for error messages
- Verify the setting name is correct (case-sensitive)
- Ensure the value type matches what the filter expects
- Test with a simple static value first

### Invalid Value
- Some filter properties have valid ranges (e.g., 0-1 for opacity)
- Check the filter's documentation for valid value ranges
- Use templates to transform the API response to the correct format

## Notes

- Filter settings are applied immediately when the URL source receives new data
- Invalid values may be silently ignored or clamped by OBS
- Not all filter properties can be modified at runtime
- This feature works with any OBS filter, including third-party filters
