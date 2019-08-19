#include <keybindings.h>

std::unordered_map<int64_t, void(*)(App)> pressBindings = {};

static std::string binaryString64(int64_t val)
{
    std::string result;
    result.resize(64, '0');

    for(int i = 0; i < 64; i++) {
        result[63 - i] = ((val >> i) & 1) ? '1' : '0';
    }

    return result;
}

static std::string binaryString32(int32_t val, uint16_t width)
{
    std::string result;
    for(int i = 0; i < 32; i++) {
        result += (val & (1 << i)) ? '1' : '0';
    }

    int32_t requiredWidth = width - result.length();

    if(requiredWidth > 0)
    {
        result.insert(result.begin(), static_cast<uint32_t>(requiredWidth), '0');
    }

    return result;
}

static std::string binaryString32(uint32_t val, uint16_t width)
{

    std::string result;
    for(int i = 0; i < 32; i++) {
        result += (val & (1 << i)) ? '1' : '0';
    }

    int32_t requiredWidth = width - result.length();

    if(requiredWidth > 0)
    {
        result.insert(result.begin(), static_cast<uint32_t>(requiredWidth), '0');
    }

    return result;
}

void setupBindings()
{
    BIND( {GLFW_KEY_ESCAPE, 0}, controls::exitApplication );
    BIND( {GLFW_KEY_S, GLFW_MOD_CONTROL }, controls::saveFile );
//    BIND( {GLFW_KEY_A, GLFW_MOD_CONTROL }, controls::selectAll );
//    BIND( {GLFW_KEY_C, GLFW_MOD_CONTROL }, controls::copySelection );
//    BIND( {GLFW_KEY_X, GLFW_MOD_CONTROL }, controls::cutSelection );
//    BIND( {GLFW_KEY_V, GLFW_MOD_CONTROL }, controls::pasteSelection );
//    BIND( {GLFW_KEY_BACKSPACE, 0 }, controls::backspace );
}

void runTests()
{
    int32_t key = 83;
    int32_t mod = 341;

    std::cout << "1u << 31 : " << binaryString32((1u << 31), 64) << std::endl;
    std::cout << "         : " << "00000000000000000000000000000001" << std::endl;
    std::bitset<32> temp(1u << 31);
    std::cout << "         : " << temp << std::endl;
    assert(1u << 31 == 0b10000000000000000000000000000000);

    assert( int64FromInt32(0, 1) == 0b0000000000000000000000000000000010000000000000000000000000000000 );
    assert( int64FromInt32(1, 0) == 0b100000000000000000000000000000000000000000000000000000000000000 );

//    std::cout << "Key/Mod       : " << binaryString32( key, 64 );
//    std::cout << binaryString32( mod, 64 ) << std::endl;
//    std::cout << "Packed string : " << binaryString64( int64FromInt32(key, mod) ) << std::endl;

    //assert(int64FromInt32(key, mod) == 0b0101001100000000000000000000000001010101010000000000000000000000)
}

constexpr int32_t int32FromInt64(int64_t val, int32_t offset = 0)
{
    return static_cast<int32_t>(val >> offset);

//    int32_t result = 0;

//    for(int i = offset; i < 32; i++)
//        result += (val | (1 << i));

//    return result;
}

constexpr int64_t int64FromInt32(int32_t a, int32_t b)
{
    int64_t result = 0;
    result = a;

    for(int32_t i = 0; i < 31; i++)
        result |= ((static_cast<int64_t>(b) & (1l << i)) << 32);

    return result;
}

void BIND(int32_t key, void(*func)(App) )
{
    pressBindings.insert( { int64FromInt32( key, 0), func } );
}

void BIND( std::tuple<int32_t, int32_t> key, void(*func)(App) )
{
    std::cout << "Binding key:" << std::get<0>(key) << " mod:" << std::get<1>(key) << std::endl;

    pressBindings.insert( { int64FromInt32( std::get<0>(key), std::get<1>(key)), func } );
}








