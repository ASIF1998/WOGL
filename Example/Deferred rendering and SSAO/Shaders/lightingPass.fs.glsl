#version 410

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D GPosition;
uniform sampler2D GNormal;
uniform sampler2D BaseColor;
uniform sampler2D SSAO;

uniform vec4 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensive;
uniform float F;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

/// Размеры окна 
#define WINDOW_WINDTH 1000.0
#define WINDOW_HEIGHT 1000.0

/// Минимальное количество локального контраста, необходимое для применения алгоритма.
/// 0.3333333 (1/3) - слишком мало
/// 0.25 (1/4) - низкое качество
/// 0.125 (1/8) - высокое качество
/// 0.0625 (1/16) - перебор
#define FXAA_EDGE_THRESHOLD 0.25 

/// Урезает алгоритм от обработки темноты.
/// 0.03125 (1/32) - видимый предел
/// 0.0625 (1/16)- высокое качество 
/// 0.083333333(1/12) - верхний предел (начало видимых нефильтрованных краев)
#define FXAA_EDGE_THRESHOLD_MIN 0.0312

/// Страхует мелкие детали от полного удаления
/// Это частично отменяет FXAA_SUBPIX_TRIM
/// 0.75 (3/4) - количество фильтрации по умолчанию
/// 0.875( 7/8) - большое количество фильтрации
/// 1.0 - нет ограничения фильтрации
#define FXAA_SUBPIX_CAP 0.75

/// Управление удалением субпиксельного сглаживания
/// 0.5 (1/2) - низкое удаление
/// 0.3333 (1/3) - среднее удаление
/// 0.2 (1/4) - удаление по умолчанию 
/// 0.125 (1/8) - высокое удаление
/// 0 - полное удаление
#define FXAA_SUBPIX_TRIM 0.25

/// На сколько ускорить поиск с помощью анизотропной фильтрации.
/// 1- нет ускорения
/// 2 - пропускать 2 текселя
/// 3 - пропускать 3 текселя
/// 4 - пропускать 4 текселя (это верхний предел)
#define FXAA_SEARCH_ACCELERATION 1

/// Управляет, когда прекратить поиск.
#define FXAA_SEARCH_THRESHOLD 0.25

/**
 * Эти определения оказывают наибольшее влияние на производительность. 
*/

/// Управляет максимальным количеством шагов поиска
/// Умножьте на FXAA_SEARCH_ACCELERATION для радиуса фильтрации.
#define FXAA_SEARCH_STEPS 32

/**
 * Необходима для вычисления яркости по формуле 0.299 * R + 0.587 * G + 0.114 * B
 * (взвешанная сумма крастного синего и зелёного компанентов).
 */
#define luma(rgb) dot(rgb, vec3(0.299, 0.587, 0.114))

#define getLumaMin(N, W, E, S, M) min(M, min(min(N, W), min(S, E)))
#define getLumaMax(N, W, E, S, M) max(M, max(max(N, W), max(S, E)))

/**
 * Функция предназначенная для уменьшения алиансинга.
 *
 * @return цвет в формате rgba
*/
vec4 fxaa()
{
    vec2 h = vec2 (
        1.0 / WINDOW_WINDTH,
        1.0 / WINDOW_HEIGHT
    );

    vec4 rgbM = texture(BaseColor, TextureCoord);
    vec4 rgbN = textureOffset(BaseColor, TextureCoord, ivec2(0, 1), 0);
    vec4 rgbW = textureOffset(BaseColor, TextureCoord, ivec2(-1, 0), 0);
    vec4 rgbS = textureOffset(BaseColor, TextureCoord, ivec2(0, -1), 0);
    vec4 rgbE = textureOffset(BaseColor, TextureCoord, ivec2(1, 0), 0);

    float lumaM = luma(rgbM.rgb);
    float lumaN = luma(rgbN.rgb);
    float lumaW = luma(rgbW.rgb);
    float lumaS = luma(rgbS.rgb);
    float lumaE = luma(rgbE.rgb);

    float maxLuma = getLumaMax(lumaM, lumaN, lumaW, lumaS, lumaE);
    float minLuma = getLumaMin(lumaM, lumaN, lumaW, lumaS, lumaE);

    float range = maxLuma - minLuma;

    /// Осуществляем проверку на разницу в локальной минимальной и максимальной яркости.
    /// Если разница ниже порога пропорциональная яркости, то это значит что видимого антиалиансинга нет.
    if (range < max(FXAA_EDGE_THRESHOLD_MIN, maxLuma * FXAA_EDGE_THRESHOLD)) {
        return rgbM;
    }

    vec4 rgbNW = textureOffset(BaseColor, TextureCoord, ivec2(-1, 1), 0);
    vec4 rgbSW = textureOffset(BaseColor, TextureCoord, ivec2(-1, -1), 0);
    vec4 rgbSE = textureOffset(BaseColor, TextureCoord, ivec2(1, -1), 0);
    vec4 rgbNE = textureOffset(BaseColor, TextureCoord, ivec2(1, 1), 0);

    float lumaNW = luma(rgbNW.rgb);
    float lumaSW = luma(rgbSW.rgb);
    float lumaSE = luma(rgbSE.rgb);
    float lumaNE = luma(rgbNE.rgb);

    vec4 rgbL = (rgbNW + rgbN + rgbNE + rgbW + rgbM + rgbE + rgbSW + rgbS + rgbSE) * 0.1111111;

    /// Далее мы оцениваем контарстность пикселя.
    /// Она расчитывается как абсолютная разница между усреднённой суммой яркости текселей
    /// (находящихся сверху, снизу, справа и слева) и текущего текселя.
    float lumaL = (lumaN +lumaW + lumaS + lumaE) * 0.25;
    float lumaRange = abs(lumaL - lumaM);

    /// Отношение контарстности текселй к локальному контарсту используется для обнаружерия алиансинга.
    float blendL = lumaRange / range;

    /// Вертикальное / горизонтальное краевое тестирование.
    float edgeVertical = abs((lumaNW + lumaSW) - (lumaNE + lumaSE));
    float edgeHorizontal = abs((lumaNW + lumaNE) - (lumaSW + lumaSE));
    bool horzSpan = edgeHorizontal >= edgeVertical;
    float lengthSign = horzSpan ? h.y : -h.x;

    if (horzSpan) {
        lumaN = lumaW;
        lumaS = lumaE;
    }

    float gradN = abs(lumaN - lumaM);
    float gradS = abs(lumaS - lumaM);
    bool pairN = gradN >= gradS;

    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;

    if (!pairN) {
        lumaN = lumaS;
        gradN = gradS;
        lengthSign *= -1.0;
    }

    vec2 offset = horzSpan ? vec2(h.x, 0.0) : vec2(0.0, h.y);
    vec2 posN = vec2 (TextureCoord + (horzSpan ? vec2(0.0, lengthSign * 0.5) : vec2(lengthSign * 0.5, 0.0)));
    vec2 posP = posN;
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;

    gradN *= FXAA_SEARCH_THRESHOLD;

    #if FXAA_SEARCH_ACCELERATION == 1
        posN += offset * vec2(-1.0, -1.0);
        posP += offset * vec2(1.0, 1.0);
    #elif FXAA_SEARCH_ACCELERATION == 2
        posN += offset * vec2(-1.5, -1.5);
        posP += offset * vec2(1.5, 1.5);
        offset *= vec2(2.0, 2.0);
    #elif FXAA_SEARCH_ACCELERATION == 3
        posN += offset * vec2(-2.0, -2.0);
        posP += offset * vec2(2.0, 2.0);
        offset *= vec2(3.0, 3.0);
    #elif FXAA_SEARCH_ACCELERATION == 4
        posN += offset * vec2(-2.5, -2.5);
        posP += offset * vec2(2.5, 2.5);
        offset *= vec2(4.0, 4.0);
    #endif

    for (uint i = 0; i < FXAA_SEARCH_STEPS; i++) {
        if (!doneN) {
            lumaEndN = luma(texture(BaseColor, posN).rgb);
            posN -= offset;
        }

        if (!doneN) {
            lumaEndP = luma(texture(BaseColor, posP).rgb);
            posP += offset;
        }

        doneN = doneN || (abs(lumaEndN - lumaN) >= gradN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradN);

        if (doneN && doneP) {
            break;
        }
    }

    float dstN = horzSpan ? TextureCoord.x - posN.x : TextureCoord.y - posN.y;
    float dstP = horzSpan ? posP.x - TextureCoord.x : posP.y - TextureCoord.y;
    bool directionN = dstN < dstP;

    lumaEndN = directionN ? lumaEndN : lumaEndP;

    if(((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) {
        lengthSign = 0.0;
    }

    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5 + (dstN * (-1.0/spanLength))) * lengthSign;

    vec4 rgbF = textureLod(BaseColor, vec2(TextureCoord.x + (horzSpan ? 0.0 : subPixelOffset),TextureCoord.y + (horzSpan ? subPixelOffset : 0.0)),0.0);

    return mix(rgbF, rgbL, blendL);
}

/**
 * Функция предназначенная для размытия фонового затенения.
 */
float blurSSAO()
{
    vec2 texelSize = 1.0 / vec2(textureSize(SSAO, 0));
    float res = 0.0;
    
    for (int x = -2; x < 2; x++) {
        for (int y = -2; y < 2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            res += texture(SSAO, TextureCoord + offset).r;
        }
    }
    
    return res / 16.0;
}

void main()
{
    vec4 position = texture(GPosition, TextureCoord);
    vec3 n = vec3(texture(GNormal, TextureCoord));
    vec3 v = vec3(-position);
    vec3 s = normalize(vec3(LightPosition - position));
    vec3 h = normalize(v + s);
    
    vec3 ambient = Ka * blurSSAO();
    vec3 diffuse = Kd * max(dot(s, n), 0.0);
    vec3 spec = Ks * pow(max(dot(n, h), 0.0), F);
    
    vec3 I = (ambient + diffuse + spec) * LightColor * LightIntensive;
    
    FragColor = fxaa() * vec4(I, 1.0);
}
