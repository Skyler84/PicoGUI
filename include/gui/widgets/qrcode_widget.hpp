#pragma once

#include "gui/widgets/widget.hpp"
#include <string>

#define EC_L 0
#define EC_M 1
#define EC_Q 2
#define EC_H 3


namespace gui
{
    class QRCodeWidget : public Widget{
    public:
        QRCodeWidget();
        template<class T>
        using matrix = std::vector<std::vector<T>>;
        void setData(std::string_view data);
        void setECLevel(int l){
            m_ecLevel = l;
            regenerate();
        }
        void setMinimumVersion(int ver){
            m_minimumVersion = ver;
            if(m_version < m_minimumVersion){
                m_version = m_minimumVersion;
                regenerate();
                requiresRelayout();
            }
            if(m_version > m_minimumVersion && m_version > m_requiredVersion){
                m_version = std::max(m_minimumVersion, m_requiredVersion);
                regenerate();
                requiresRelayout();
            }
        }
        void setScale(int s){
            m_scale = s;
            requiresRedraw();
            requiresRelayout();
        }
        int getVersion() const{
            return m_version;
        }
        int getScale() const{
            return m_scale;
        }
        int getArea() const{
            return (17 + 4*m_version + m_borderSize*2) * m_scale;
        }
        void setBorderSize(int s){
            m_borderSize = s;
        }
        int getBorderSize() const{
            return m_borderSize;
        }
    protected:
        void redraw(Graphics&, bool) override;
    private:
        std::vector<char> m_content;
        int m_minimumVersion;
        int m_requiredVersion;
        int m_version = 1;
        std::vector<bool> m_qrcodeData;
        int m_scale = 1;
        int m_mask = 0;
        int m_borderSize = 3;
        int m_ecLevel = 0;

        void regenerate();

        Point2 getNextPixelPos(Point2);
        static int getActiveSizeForVersion(int version);
        static int getNumCountBitsForEncoding(int version, int encoding);
        static int getNumAlignmentPatterns(int version);
        static int getNumAlignmentPatternIdx(int version);
        static int getDataCapacity(int version);
        static bool isValidDataModule(int version, Point2 p);
        static bool getFixedPatternValue(int version, Point2 p);
        static int getAlignmentPatternPos(int version, int patIdx);
        static bool getMask(int maskNo, const Point2 &p);
        static int getFormatInfo(int mask, int ec);


    };
} // namespace gui
