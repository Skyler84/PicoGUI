#include "gui/widgets/qrcode_widget.hpp"


#define ENCODING_NUM 0
#define ENCODING_ALN 1
#define ENCODING_BIN 2
#define ENCODING_KAN 3


const int data_capacity_bits[40][4] = {
    {152, 128, 104, 72},
    {272, 224, 176, 128},
    {440, 352, 272, 208},
    {640, 512, 384, 288},
    {864, 688, 496, 368},
    {1088, 864, 608, 480},
    {1248, 992, 704, 528},
    {1552, 1232, 880, 688},
    {1856, 1456, 1056, 800},
    {2192, 1728, 1232, 976},
};

const int alignmentPos[40][7] = {
    {   6,},
    {   6,	18  },
    {   6,	22  },
    {   6,	26  },
    {   6,	30  },
    {   6,	34  },
    {   6,	22,	38  },
    {   6,	24,	42  },
    {   6,	26,	46  },
    {   6,	28,	50  },
    {   6,	30,	54  },
    {   6,	32,	58  },
    {   6,	34,	62  },
    {   6,	26,	46,	66  },
    {   6,	26,	48,	70  },
    {   6,	26,	50,	74  },
    {   6,	30,	54,	78  },
    {   6,	30,	56,	82  },
    {   6,	30,	58,	86  },
    {   6,	34,	62,	90  },
    {   6,	28,	50,	72,	94  },
    {   6,	26,	50,	74,	98  },
    {   6,	30,	54,	78,	102 },
    {   6,	28,	54,	80,	106 },
    {   6,	32,	58,	84,	110 },
    {   6,	30,	58,	86,	114 },
    {   6,	34,	62,	90,	118 },
    {   6,	26,	50,	74,	98,	122 },
    {   6,	30,	54,	78,	102,126 },
    {   6,	26,	52,	78,	104,130 },
    {   6,	30,	56,	82,	108,134 },
    {   6,	34,	60,	86,	112,138 },
    {   6,	30,	58,	86,	114,142 },
    {   6,	34,	62,	90,	118,146 },
    {   6,	30,	54,	78,	102,126,150  },
    {   6,	24,	50,	76,	102,128,154  },
    {   6,	28,	54,	80,	106,132,158  },
    {   6,	32,	58,	84,	110,136,162  },
    {   6,	26,	54,	82,	110,138,166  },
    {   6,	30,	58,	86,	114,142,170  },
};

const int version_information[] = {
    0b000111110010010100,
    0b001000010110111100,
    0b001001101010011001,
    0b001010010011010011,
    0b001011101111110110,
    0b001100011101100010,
    0b001101100001000111,
    0b001110011000001101,
    0b001111100100101000,
    0b010000101101111000,
    0b010001010001011101,
    0b010010101000010111,
    0b010011010100110010,
    0b010100100110100110,
    0b010101011010000011,
    0b010110100011001001,
    0b010111011111101100,
    0b011000111011000100,
    0b011001000111100001,
    0b011010111110101011,
    0b011011000010001110,
    0b011100110000011010,
    0b011101001100111111,
    0b011110110101110101,
    0b011111001001010000,
    0b100000100111010101,
    0b100001011011110000,
    0b100010100010111010,
    0b100011011110011111,
    0b100100101100001011,
    0b100101010000101110,
    0b100110101001100100,
    0b100111010101000001,
    0b101000110001101001,
};
//format_information[mask_pattern][ECC_level]
const int format_information[8][4] = {
    {0b111011111000100, 0b101010000010010, 0b011010101011111, 0b001011010001001},
    {0b111001011110011, 0b101000100100101, 0b011000001101000, 0b001001110111110},
    {0b111110110101010, 0b101111001111100, 0b011111100110001, 0b001110011100111},
    {0b111100010011101, 0b101101101001011, 0b011101000000110, 0b001100111010000},
    {0b110011000101111, 0b100010111111001, 0b010010010110100, 0b000011101100010},
    {0b110001100011000, 0b100000011001110, 0b010000110000011, 0b000001001010101},
    {0b110110001000001, 0b100111110010111, 0b010111011011010, 0b000110100001100},
    {0b110100101110110, 0b100101010100000, 0b010101111101101, 0b000100000111011},
};

/**ec_tables[ver][ec_level][val]
 * enum val{
 *      ec_words_per_block = 0
 *      num_blocks_group_1 = 1
 *      data_words_group_1 = 2
 *      num_blocks_group_2 = 3
 *      data_words_group_2 = 4
 * }
 */
const int ec_tables[40][4][5]{
    {
        { 7,	1,	19		},
        { 10,	1,	16		},
        { 13,	1,	13		},
        { 17,	1,	9		},
    },{
        { 10,	1,	34		},
        { 16,	1,	28		},
        { 22,	1,	22		},
        { 28,	1,	16		},
    },{
        { 15,	1,	55		},
        { 26,	1,	44		},
        { 18,	2,	17		},
        { 22,	2,	13		},
    },{
        { 20,	1,	80		},
        { 18,	2,	32		},
        { 26,	2,	24		},
        { 16,	4,	9		},
    },{
        { 26,	1,	108		},
        { 24,	2,	43		},
        { 18,	2,	15,	2,	16},
        { 22,	2,	11,	2,	12},
    },{
        { 18,	2,	68		},
        { 16,	4,	27		},
        { 24,	4,	19		},
        { 28,	4,	15		},
    },{
        { 20,	2,	78		},
        { 18,	4,	31		},
        { 18,	2,	14,	4,	15},
        { 26,	4,	13,	1,	14},
    },{
        { 24,	2,	97		},
        { 22,	2,	38,	2,	39},
        { 22,	4,	18,	2,	19},
        { 26,	4,	14,	2,	15},
    },{
        { 30,	2,	116		},
        { 22,	3,	36,	2,	37},
        { 20,	4,	16,	4,	17},
        { 24,	4,	12,	4,	13},
    },{
        { 18,	2,	68,	2,	69},
        { 26,	4,	43,	1,	44},
        { 24,	6,	19,	2,	20},
        { 28,	6,	15,	2,	16},
    },{
        { 20,	4,	81		},
        { 30,	1,	50,	4,	51},
        { 28,	4,	22,	4,	23},
        { 24,	3,	12,	8,	13},
    },{
        { 24,	2,	92,	2,	93},
        { 22,	6,	36,	2,	37},
        { 26,	4,	20,	6,	21},
        { 28,	7,	14,	4,	15},
    },{
        { 26,	4,	107		},
        { 22,	8,	37,	1,	38},
        { 24,	8,	20,	4,	21},
        { 22,	12,	11,	4,	12},
    },{
        { 30,	3,	115,	1,	116},
        { 24,	4,	40,	5,	41},
        { 20,	11,	16,	5,	17},
        { 24,	11,	12,	5,	13},
    },{
        { 22,	5,	87,	1,	88},
        { 24,	5,	41,	5,	42},
        { 30,	5,	24,	7,	25},
        { 24,	11,	12,	7,	13},
    },{
        { 24,	5,	98,	1,	99},
        { 28,	7,	45,	3,	46},
        { 24,	15,	19,	2,	20},
        { 30,	3,	15,	13,	16},
    },{
        { 28,	1,	107,	5,	108},
        { 28,	10,	46,	1,	47},
        { 28,	1,	22,	15,	23},
        { 28,	2,	14,	17,	15},
    },{
        { 30,	5,	120,	1,	121},
        { 26,	9,	43,	4,	44},
        { 28,	17,	22,	1,	23},
        { 28,	2,	14,	19,	15},
    },{
        { 28,	3,	113,	4,	114},
        { 26,	3,	44,	11,	45},
        { 26,	17,	21,	4,	22},
        { 26,	9,	13,	16,	14},
    },{
        { 28,	3,	107,	5,	108},
        { 26,	3,	41,	13,	42},
        { 30,	15,	24,	5,	25},
        { 28,	15,	15,	10,	16},
    },{
        { 28,	4,	116,	4,	117},
        { 26,	17,	42		},
        { 28,	17,	22,	6,	23},
        { 30,	19,	16,	6,	17},
    },{
        { 28,	2,	111,	7,	112},
        { 28,	17,	46		},
        { 30,	7,	24,	16,	25},
        { 24,	34,	13		},
    },{
        { 30,	4,	121,	5,	122},
        { 28,	4,	47,	14,	48},
        { 30,	11,	24,	14,	25},
        { 30,	16,	15,	14,	16},
    },{
        { 30,	6,	117,	4,	118},
        { 28,	6,	45,	14,	46},
        { 30,	11,	24,	16,	25},
        { 30,	30,	16,	2,	17},
    },{
        { 26,	8,	106,	4,	107},
        { 28,	8,	47,	13,	48},
        { 30,	7,	24,	22,	25},
        { 30,	22,	15,	13,	16},
    },{
        { 28,	10,	114,	2,	115},
        { 28,	19,	46,	4,	47},
        { 28,	28,	22,	6,	23},
        { 30,	33,	16,	4,	17},
    },{
        { 30,	8,	122,	4,	123},
        { 28,	22,	45,	3,	46},
        { 30,	8,	23,	26,	24},
        { 30,	12,	15,	28,	16},
    },{
        { 30,	3,	117,	10,	118},
        { 28,	3,	45,	23,	46},
        { 30,	4,	24,	31,	25},
        { 30,	11,	15,	31,	16},
    },{
        { 30,	7,	116,	7,	117},
        { 28,	21,	45,	7,	46},
        { 30,	1,	23,	37,	24},
        { 30,	19,	15,	26,	16},
    },{
        { 30,	5,	115,	10,	116},
        { 28,	19,	47,	10,	48},
        { 30,	15,	24,	25,	25},
        { 30,	23,	15,	25,	16},
    },{
        { 30,	13,	115,	3,	116},
        { 28,	2,	46,	29,	47},
        { 30,	42,	24,	1,	25},
        { 30,	23,	15,	28,	16},
    },{
        { 30,	17,	115		},
        { 28,	10,	46,	23,	47},
        { 30,	10,	24,	35,	25},
        { 30,	19,	15,	35,	16},
    },{
        { 30,	17,	115,	1,	116},
        { 28,	14,	46,	21,	47},
        { 30,	29,	24,	19,	25},
        { 30,	11,	15,	46,	16},
    },{
        { 30,	13,	115,	6,	116},
        { 28,	14,	46,	23,	47},
        { 30,	44,	24,	7,	25},
        { 30,	59,	16,	1,	17},
    },{
        { 30,	12,	121,	7,	122},
        { 28,	12,	47,	26,	48},
        { 30,	39,	24,	14,	25},
        { 30,	22,	15,	41,	16},
    },{
        { 30,	6,	121,	14,	122},
        { 28,	6,	47,	34,	48},
        { 30,	46,	24,	10,	25},
        { 30,	2,	15,	64,	16},
    },{
        { 30,	17,	122,	4,	123},
        { 28,	29,	46,	14,	47},
        { 30,	49,	24,	10,	25},
        { 30,	24,	15,	46,	16},
    },{
        { 30,	4,	122,	18,	123},
        { 28,	13,	46,	32,	47},
        { 30,	48,	24,	14,	25},
        { 30,	42,	15,	32,	16},
    },{
        { 30,	20,	117,	4,	118},
        { 28,	40,	47,	7,	48},
        { 30,	43,	24,	22,	25},
        { 30,	10,	15,	67,	16},
    },{
        { 30,	19,	118,	6,	119},
        { 28,	18,	47,	31,	48},
        { 30,	34,	24,	34,	25},
        { 30,	20,	15,	61,	16},
    }
};
// #include <ctype>

constexpr uint8_t mul2(uint8_t v){
    if(v&0x80)
        v = v*2 ^ 0x1d; //285 missing its top bit 
    else
        v = v*2; 
    return v;
}

constexpr std::array<uint8_t, 256> create_antilog_lookup(){
    std::array<uint8_t, 256> out{};
    uint8_t v = 1;
    for(auto &e : out){
        e = v;
        v = mul2(v);
    }
    return out; 
};
constexpr std::array<uint8_t, 256> create_log_lookup(){
    std::array<uint8_t, 256> out{};
    uint8_t v = 1;
    for(int i = 0; i < 256; i++){
        out[v] = i;
        v = mul2(v);
    }
    return out;
};

constexpr std::array<uint8_t, 256> antilog_lookup = create_antilog_lookup();
constexpr std::array<uint8_t, 256> log_lookup = create_log_lookup();

uint8_t mul(uint8_t a, uint8_t b){
    if(!(a&&b)) return 0;
    return antilog_lookup[(log_lookup[a]+log_lookup[b])%255];
}
uint8_t div(uint8_t a, uint8_t b){
    assert(b);
    if(!a) return 0;
    return antilog_lookup[(255+log_lookup[a]-log_lookup[b])%255];
}

std::vector<uint8_t> mul_polynomials(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b){
    std::vector<uint8_t> out{};
    out.resize(a.size() + b.size() - 1);
    for(int i = 0; i < a.size(); i++){
        for(int j = 0; j < b.size(); j++){
            int pos = i+j;
            uint8_t prod = mul(a[i], b[j]);
            out[pos] ^= prod;
        }
    }
    return out;
}

std::vector<uint8_t> div_polynomials(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b){
    std::vector<uint8_t> rem{};
    rem.resize(a.size() + b.size());
    int as = a.size();
    int bs = b.size();
    for(int i = 0; i < a.size(); i++)
        rem[b.size() - 1 + i] = a[i];
    
    for(int i = 0; i < a.size(); i++){
        uint8_t divisor = div(rem[as+bs-i-2], b[bs-1]);
        for(int j = 0; j < b.size(); j++){
            int ri = rem.size() - (i + j + 1);
            int bi = b.size() - (j+1);
            uint8_t bv = b[bi];
            rem[ri-1] ^= mul(bv, divisor);
        }
    }

    rem.resize(b.size() - 1);
    return rem;
}

std::vector<uint8_t> create_generator(uint8_t i){
    assert(i);
    if(i == 1)
        return {1,1};
    return mul_polynomials({antilog_lookup[i-1], 1}, create_generator(i-1));
}

using namespace gui;

QRCodeWidget::QRCodeWidget(){
    m_content.clear();
    m_minimumVersion = -1;
    m_requiredVersion = 1;
    m_version = 1;

    regenerate();
}

int encoding_for_byte(char c){
    if(c >= '0' && c <= '9')
        return ENCODING_NUM;
    if(c >= 'A' && c <= 'Z' 
        || c == ' '
        || c == '$'
        || c == '*'
        || c == '+'
        || c == '-'
        || c == '.'
        || c == '/'
        || c == ':' ) 
        return ENCODING_ALN;
    else
        return ENCODING_BIN;
}


void QRCodeWidget::setData(std::string_view data){
    printf("[QRCODE] setData()\n");
    m_content.clear();
    m_content.reserve(data.size());
    for(auto e : data){
        m_content.push_back(e);
    }
    regenerate();
}

void QRCodeWidget::regenerate(){
    printf("[QRCODE] regenerate()\n");
    //calc version
    int ver = 1;
    while(data_capacity_bits[ver-1][m_ecLevel] < (4 + getNumCountBitsForEncoding(ver, ENCODING_BIN) + m_content.size()*8 + 4))ver++;
    m_requiredVersion = ver;
    m_version = std::max(m_minimumVersion, ver);
    int capacity = data_capacity_bits[m_version-1][m_ecLevel];

    std::vector<std::vector<uint8_t>> data_blocks;
    auto ectab = ec_tables[m_version-1][m_ecLevel];
    int nbg1 = ectab[1];
    int nbg2 = ectab[3];
    printf("[QRCODE] using version %d, min %d, req %d\n", m_version, m_minimumVersion, m_requiredVersion);
    //allocate block vectors
    for(int n = 0; n < 2; n++){
        for(int i = 0;i < ectab[1+n*2]; i++){
            int bs = ectab[2+n*2];
            std::vector<uint8_t> v;
            v.resize(bs);
            data_blocks.emplace_back(std::move(v));
        }
    }

    m_qrcodeData.clear();
    int bit_cnt = 0;

    int block_num = 0;
    int block_byte = 0;
    int group_block_num = 0;
    int group_num = 0;
    uint8_t codeword = 0;
    auto writeDataBlockBits = [&](int b, int n, bool _b = false){
        assert(n);
        b <<= (8*sizeof(b)-n);
        int mask = 1<<(8*sizeof(b)-1);
        while(n--){
            bool bit = b&mask;
            b <<= 1;
            codeword <<=1;
            codeword |= bit;
            bit_cnt++;
            if(bit_cnt%8 || _b)
                continue;
            data_blocks[block_num][ectab[2+2*group_num] - 1 - block_byte++] = codeword;
            if(block_byte != ectab[2+2*group_num])
                continue;
            block_byte = 0;
            block_num++;
            group_block_num++;
            if(group_block_num < ectab[1+group_num*2])
                continue;
            group_block_num = 0;
            group_num++;
            assert(group_num < 2);
        }
    };
    auto writeCodeword = [&](uint8_t codeword){
        for(uint8_t i = 0x80; i; i >>= 1){
            m_qrcodeData.push_back(codeword&i);
        }
    };
    auto writeBlocksInterleaved = [&](const std::vector<std::vector<uint8_t>>&bs){
        int i = 0;
        int numBlocks = bs.size();
        while(numBlocks){
            for(const auto &b : bs){
                if(b.size() == i){
                    numBlocks--;
                    continue;
                }
                if(b.size() < i)
                    continue;
                writeCodeword(b[b.size() - 1 - i]);
                // printf("wrote codeword %02x\n", b[b.size() - 1 - i]);
            }
            i++;
        }
    };

    
    //write header
    writeDataBlockBits(0b0100, 4);
    //write length
    writeDataBlockBits(m_content.size(), getNumCountBitsForEncoding(m_version, ENCODING_BIN));
    
    //write data
    for(auto e : m_content){
        writeDataBlockBits(e, 8);
    }
    //write terminator
    writeDataBlockBits(0, 4);
    if(bit_cnt % 8){
        writeDataBlockBits(0, 8-bit_cnt%8);
    }
    bool b = true;
    while(bit_cnt < capacity){
        writeDataBlockBits(b?0xec:0x11, 8);
        b = !b;
    }

    std::vector<std::vector<uint8_t>> ecc_blocks{};
    auto gen = create_generator(ectab[0]);
    for(auto &b : data_blocks){
        auto ecc_block = div_polynomials(b, gen);
        ecc_blocks.emplace_back(std::move(ecc_block));
    }

    writeBlocksInterleaved(data_blocks);
    writeBlocksInterleaved(ecc_blocks);
    
    requiresRedraw();
}

std::vector<bool> createSegmentBinary(const char *start, const char *end){
    int len = end-start;
    std::vector<bool> out;

    // out.reserve(4 + len + );
    return out;
}

void QRCodeWidget::redraw(Graphics &g, bool){
    printf("[QRCODE] redraw()\n");
    Point2 o{0,0};
    int size = getActiveSizeForVersion(m_version);
    printf("[QRCODE] ver %d, scale %d\n", this->m_version, this->m_scale);
    //draw fixed patterns

    Point2 bo{m_borderSize*m_scale, m_borderSize*m_scale};
    Point2 tl, br;
    bo += getPos();
    g.set_pen(255);
    g.rectangle(pimoroni::Rect{getPos(), bo + Point2{getArea(), getArea()}});
    for(int i = 0; i < size; i++){
        g.set_pen(i%2?255:0);
        tl = {6  , i  }; tl *= m_scale; tl += bo;
        br = {6+1, i+1}; br *= m_scale; br += bo;
        g.rectangle(pimoroni::Rect{tl, br});
        tl = {i  , 6  }; tl *= m_scale; tl += bo;
        br = {i+1, 6+1}; br *= m_scale; br += bo;
        g.rectangle(pimoroni::Rect{tl, br});
    }
    for(int i = 3; i; i--){
        g.set_pen(i%2?0:255);
        tl = {3-i  , 3-i  }; tl *= m_scale; tl += bo;
        br = {3+i+1, 3+i+1}; br *= m_scale; br += bo;
        g.rectangle(pimoroni::Rect{tl, br});
        tl = {size-3-i-1, 3-i  }; tl *= m_scale; tl += bo;
        br = {size-3+i  , 3+i+1}; br *= m_scale; br += bo;
        g.rectangle(pimoroni::Rect{tl, br});
        tl = {3-i  , size-3-i-1}; tl *= m_scale; tl += bo;
        br = {3+i+1, size-3+i  }; br *= m_scale; br += bo;
        g.rectangle(pimoroni::Rect{tl, br});
    }

    int aligPatNum = getNumAlignmentPatternIdx(m_version);
    if(m_version > 1)
        for(int i = 0; i < aligPatNum; i++){
            for(int j = 0; j < aligPatNum; j++){
                if(i == 0 && (j == 0 || j == aligPatNum-1))
                    continue;
                if(j == 0 && (i == 0 || i == aligPatNum-1))
                    continue;
                int iPos = getAlignmentPatternPos(m_version, i);
                int jPos = getAlignmentPatternPos(m_version, j);
                for(int i = 3; i;){
                    g.set_pen(i%2?0:255);
                    --i;
                    tl = {-i,  -i}; tl *= m_scale; tl += Point2{iPos, jPos}*m_scale + bo;
                    br = {+i+1,+i+1}; br *= m_scale; br += Point2{iPos, jPos}*m_scale + bo;
                    g.rectangle(pimoroni::Rect{tl, br});
                }
            }
        }
    //draw version info
    if(m_version >= 7){
        assert(false && "NOT COMPLETED YET");
    }
    //draw formatting info
    {
        int formatInfo = getFormatInfo(m_mask, m_ecLevel);
        int i = 0;
        auto drawModule = [&](Point2 p){
            // g.set_pen(c);
            Point2 size = {m_scale,m_scale};
            p *= m_scale; p += bo;
            g.rectangle(pimoroni::Rect{p, p+size});
        };

        auto formatBit = [=](int bit){
            return formatInfo & (1<<(14-bit));
        };

        for(; i < 6; i++){
            g.set_pen(formatBit(i)?0:255);
            drawModule({i, 8});
            drawModule({8, size-i-1});
        }
        i = 6;
        g.set_pen(formatBit(i)?0:255);
        drawModule({i+1,8});
        drawModule({8,size-i-1});
        for(i = 7; i < 9; i++){
            g.set_pen(formatBit(i)?0:255);
            drawModule({8,  15-i});
            drawModule({size-1-(14-i),8});
        }
        for(i = 9; i < 15; i++){
            g.set_pen(formatBit(i)?0:255);
            drawModule({8,  14-i});
            drawModule({size-1-(14-i),8});
        }
        g.set_pen(0);
        drawModule({8, size-7-1});
    }
    //draw data modules
    {
        o = {size-1, size-1};
        int i = 0;
        do{
            // printf("{%d,%d}\n",o.x,o.y);
            bool data = m_qrcodeData[i++];
            bool mask = getMask(m_mask, o);
            g.set_pen(data^mask?0:255);
            Point2 tl = {0,0}; tl += o; tl *= m_scale; tl += bo;
            Point2 br = {1,1}; br += o; br *= m_scale; br += bo;
            g.rectangle(pimoroni::Rect{tl, br});
            o = getNextPixelPos(o);
        }while(o.x >= 0);
    }
    printf("[QRCODE] redraw() complete\n");

}

Point2 QRCodeWidget::getNextPixelPos(Point2 p){
    int size = getActiveSizeForVersion(m_version);
    Point2 offset{};
    if(p.x > 6){
        offset.x = 1; //treat past left timing indicator
        p -= offset;
    }
    do{
        switch(p.x % 4){
            case 0: //y-1, x+1
                p.y++; p.x++; break;
            case 1: //x-1
                p.x--; break;
            case 2: //y+1, x+1
                p.y--; p.x++; break;
            case 3: //x-1
                p.x--; break;
            if(p.x == 5)
                offset.x = 0;
        } 
        if(p.y < 0){
            p.x -=2;
            p.y = 0;
        }else if(p.y >= size){
            p.x -=2;
            p.y = size-1;
        }
        if(p.x <= 5)
            offset.x = 0;
    }while(!isValidDataModule(m_version, p+offset) && p.x >= 0);

    return p+offset;
}

int QRCodeWidget::getActiveSizeForVersion(int version) {
    return version * 4 + 17;
}

int QRCodeWidget::getNumCountBitsForEncoding(int version, int encoding){
    assert(version >= 1 && version <= 40);
    assert(encoding >= 0 && encoding <= 3);
    if(version <= 9) return ((int[]){10,9,8,8})[encoding];
    if(version <= 26) return ((int[]){12,11,16,10})[encoding];
    if(version <= 40) return ((int[]){14,13,16,12})[encoding];
    assert(false);
}

int QRCodeWidget::getNumAlignmentPatterns(int version){
    assert(version >= 1 && version <= 40);
    if(version == 1) return 0;
    int n = version/7;
    return (n+2)*(n+2) - 3;
}

int QRCodeWidget::getNumAlignmentPatternIdx(int version){
    assert(version >= 1 && version <= 40);
    int n = version/7;
    return (n+2);
}

int QRCodeWidget::getDataCapacity(int version){
    assert(version >= 1 && version <= 40);
    int size = getActiveSizeForVersion(version);
    int numAlignment = 0;
    int totalArea = size*size;
    constexpr int finderArea = 3*8*8;
    constexpr int formatArea = 7*4;
    int alignmentArea = (5*5)*numAlignment;
    int timingArea = 2*(size-16);
    int usableArea = totalArea - (finderArea + formatArea + timingArea + 1 + 25*getNumAlignmentPatterns(version) );
    return usableArea;
}

bool QRCodeWidget::isValidDataModule(int version, Point2 p){
    if(p.x == 6 || p.y == 6)
        return false;
    int size = getActiveSizeForVersion(version);
    if(p.x <= 8 && (p.y <= 8 || p.y >= size-8))
        return false;
    if(p.y <= 8 && (p.x <= 8 || p.x >= size-8))
        return false;
    if(version == 1) return true;
    int aligPatNum = getNumAlignmentPatternIdx(version);
    for(int i = 0; i < aligPatNum; i++){
        for(int j = 0; j < aligPatNum; j++){
            if(i == 0 && (j == 0 || j == aligPatNum-1))
                continue;
            if(j == 0 && (i == 0 || i == aligPatNum-1))
                continue;
            int iPos = getAlignmentPatternPos(version, i);
            int jPos = getAlignmentPatternPos(version, j);
            if(p.x >= iPos - 2 && p.x <= iPos+2 && p.y >= jPos - 2 && p.y <= jPos+2)
                return false;
        }
    }
    return true;
}

bool QRCodeWidget::getFixedPatternValue(int version, Point2 p){
    int size = getActiveSizeForVersion(version);

    auto getFinderPatternVal = [](Point2 p, bool corner){
        p.x = abs(p.x);
        p.y = abs(p.y);
        int d = std::max(p.x, p.y);
        if(corner)
            return !(d%2)^(d==0);
        else
            return (d%2);      
        };

    if(p.x <= 8 && p.y <= 8){
        p.x -= 3;
        p.y -= 3;
        return getFinderPatternVal(p, true);
    }
    if(p.x <= 8 && p.y >= size-8){
        p.x -= 3;
        p.y -= size-3-1;
        return getFinderPatternVal(p, true);
    }
    if(p.y <= 8 && p.x >= size-8){
        p.y -= 3;
        p.x -= size-3-1;
        return getFinderPatternVal(p, true);
    }
    if(p.x == 6 || p.y == 6)
        return (p.x+p.y)%2;

    assert(version != 1);
    int aligPatNum = getNumAlignmentPatternIdx(version);
    for(int i = 0; i < aligPatNum; i++){
        for(int j = 0; j < aligPatNum; j++){
            if(i == 0 && (j == 0 || j == aligPatNum-1))
                continue;
            if(j == 0 && (i == 0 || i == aligPatNum-1))
                continue;
            int iPos = getAlignmentPatternPos(version, i);
            int jPos = getAlignmentPatternPos(version, j);
            if(p.x >= iPos - 2 && p.x <= iPos+2 && p.y >= jPos - 2 && p.y <= jPos+2){
                p.x -= iPos;
                p.y -= jPos;
                return getFinderPatternVal(p, false);
            }
        }
    }
    assert(false && "Invalid point for fixed patterns");
}

int QRCodeWidget::getAlignmentPatternPos(int version, int patIdx){
    int n7 = version/7;
    assert(version >= 1 && version <= 40);
    assert(patIdx <= n7+2);

    return alignmentPos[version-1][patIdx];

}

bool QRCodeWidget::getMask(int maskNo, const Point2 &p){
    switch (maskNo^0b010)
    {
    case 0: return p.x % 3 == 0;
    case 1: return (p.x + p.y) % 3 == 0;
    case 2: return (p.x + p.y) % 2 == 0;
    case 3: return p.y % 2 == 0;
    case 4: return ((p.y*p.x) % 3 + p.y*p.x) % 2 == 0;
    case 5: return ((p.y*p.x) % 3 + p.y+p.x) % 2 == 0;
    case 6: return (p.y/2 + p.x/3) % 2 == 0;
    case 7: return (p.y*p.x)%2 + (p.y*p.x)%3 == 0;
    default: assert(false && "Invalid mask pattern no");
    }
}

int QRCodeWidget::getFormatInfo(int mask, int ec){
    return format_information[mask][ec];
}

