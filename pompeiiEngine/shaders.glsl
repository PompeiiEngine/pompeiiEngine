#define PI                 3.14159265359
/** @public-api */
#define HALF_PI            1.570796327

#define MEDIUMP_FLT_MAX    65504.0
#define MEDIUMP_FLT_MIN    0.00006103515625

#ifdef TARGET_MOBILE
#define FLT_EPS            MEDIUMP_FLT_MIN
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)
#else
#define FLT_EPS            1e-5
#define saturateMediump(x) x
#endif

#define saturate(x)        clamp(x, 0.0, 1.0)


float luminance(const vec3 linear) {
    return dot(linear, vec3(0.2126, 0.7152, 0.0722));
}











//------------------------------------------------------------------------------
// Tone-mapping configuration
//------------------------------------------------------------------------------

// Operators for LDR output
#define TONE_MAPPING_LINEAR           0
#define TONE_MAPPING_REINHARD         1
#define TONE_MAPPING_UNREAL           2
#define TONE_MAPPING_ACES             3

// Operators for HDR output
#define TONE_MAPPING_ACES_REC2020_1K  4

// Debug operators
#define TONE_MAPPING_DISPLAY_RANGE    9

#ifdef TARGET_MOBILE
    #ifdef HAS_HARDWARE_CONVERSION_FUNCTION
        #define TONE_MAPPING_OPERATOR TONE_MAPPING_ACES
    #else
        #define TONE_MAPPING_OPERATOR TONE_MAPPING_UNREAL
    #endif
#else
    #define TONE_MAPPING_OPERATOR     TONE_MAPPING_ACES
#endif

//------------------------------------------------------------------------------
// Tone-mapping operators for LDR output
//------------------------------------------------------------------------------

vec3 Tonemap_Linear(const vec3 x) {
    return x;
}

vec3 Tonemap_Reinhard(const vec3 x) {
    // Reinhard et al. 2002, "Photographic Tone Reproduction for Digital Images", Eq. 3
    return x / (1.0 + luminance(x));
}

vec3 Tonemap_Unreal(const vec3 x) {
    // Unreal, Documentation: "Color Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    return x / (x + 0.155) * 1.019;
}

vec3 Tonemap_ACES(const vec3 x) {
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

//------------------------------------------------------------------------------
// Tone-mapping operators for HDR output
//------------------------------------------------------------------------------

#if TONE_MAPPING_OPERATOR == TONE_MAPPING_ACES_REC2020_1K
vec3 Tonemap_ACES_Rec2020_1k(const vec3 x) {
    // Narkowicz 2016, "HDR Display – First Steps"
    const float a = 15.8;
    const float b = 2.12;
    const float c = 1.2;
    const float d = 5.92;
    const float e = 1.9;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}
#endif

//------------------------------------------------------------------------------
// Debug tone-mapping operators, for LDR output
//------------------------------------------------------------------------------

/**
 * Converts the input HDR RGB color into one of 16 debug colors that represent
 * the pixel's exposure. When the output is cyan, the input color represents
 * middle gray (18% exposure). Every exposure stop above or below middle gray
 * causes a color shift.
 *
 * The relationship between exposures and colors is:
 *
 * -5EV  - black
 * -4EV  - darkest blue
 * -3EV  - darker blue
 * -2EV  - dark blue
 * -1EV  - blue
 *  OEV  - cyan
 * +1EV  - dark green
 * +2EV  - green
 * +3EV  - yellow
 * +4EV  - yellow-orange
 * +5EV  - orange
 * +6EV  - bright red
 * +7EV  - red
 * +8EV  - magenta
 * +9EV  - purple
 * +10EV - white
 */
#if TONE_MAPPING_OPERATOR == TONE_MAPPING_DISPLAY_RANGE
vec3 Tonemap_DisplayRange(const vec3 x) {
    // 16 debug colors + 1 duplicated at the end for easy indexing
    const vec3 debugColors[17] = vec3[](
         vec3(0.0, 0.0, 0.0),         // black
         vec3(0.0, 0.0, 0.1647),      // darkest blue
         vec3(0.0, 0.0, 0.3647),      // darker blue
         vec3(0.0, 0.0, 0.6647),      // dark blue
         vec3(0.0, 0.0, 0.9647),      // blue
         vec3(0.0, 0.9255, 0.9255),   // cyan
         vec3(0.0, 0.5647, 0.0),      // dark green
         vec3(0.0, 0.7843, 0.0),      // green
         vec3(1.0, 1.0, 0.0),         // yellow
         vec3(0.90588, 0.75294, 0.0), // yellow-orange
         vec3(1.0, 0.5647, 0.0),      // orange
         vec3(1.0, 0.0, 0.0),         // bright red
         vec3(0.8392, 0.0, 0.0),      // red
         vec3(1.0, 0.0, 1.0),         // magenta
         vec3(0.6, 0.3333, 0.7882),   // purple
         vec3(1.0, 1.0, 1.0),         // white
         vec3(1.0, 1.0, 1.0)          // white
    );

    // The 5th color in the array (cyan) represents middle gray (18%)
    // Every stop above or below middle gray causes a color shift
    float v = log2(luminance(x) / 0.18);
    v = clamp(v + 5.0, 0.0, 15.0);
    int index = int(v);
    return mix(debugColors[index], debugColors[index + 1], v - float(index));
}
#endif

//------------------------------------------------------------------------------
// Tone-mapping dispatch
//------------------------------------------------------------------------------

/**
 * Tone-maps the specified RGB color. The input color must be in linear HDR and
 * pre-exposed. Our HDR to LDR tone mapping operators are designed to tone-map
 * the range [0..~8] to [0..1].
 */
vec3 tonemap(const vec3 x) {
#if TONE_MAPPING_OPERATOR == TONE_MAPPING_LINEAR
    return Tonemap_Linear(x);
#elif TONE_MAPPING_OPERATOR == TONE_MAPPING_REINHARD
    return Tonemap_Reinhard(x);
#elif TONE_MAPPING_OPERATOR == TONE_MAPPING_UNREAL
    return Tonemap_Unreal(x);
#elif TONE_MAPPING_OPERATOR == TONE_MAPPING_ACES
    return Tonemap_ACES(x);
#elif TONE_MAPPING_OPERATOR == TONE_MAPPING_ACES_REC2020_1K
    return Tonemap_ACES_Rec2020_1k(x);
#elif TONE_MAPPING_OPERATOR == TONE_MAPPING_DISPLAY_RANGE
    return Tonemap_DisplayRange(x);
#endif
}

//------------------------------------------------------------------------------
// Processing tone-mappers
//------------------------------------------------------------------------------

vec3 Tonemap_ReinhardWeighted(const vec3 x, float weight) {
    // Weighted Reinhard tone-mapping operator designed for post-processing
    // This tone-mapping operator is invertible
    return x * (weight / (max3(x) + 1.0));
}

vec3 Tonemap_ReinhardWeighted_Invert(const vec3 x) {
    // Inverse Reinhard tone-mapping operator, designed to be used in conjunction
    // with the weighted Reinhard tone-mapping operator
    return x / (1.0 - max3(x));
}