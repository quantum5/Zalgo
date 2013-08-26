#define RID_ICON 1
#define RID_XSAMPA 2
#define RID_INIT 3
#define RID_LOOSE 4

inline void GetMessageFont(LOGFONT &lf) {
    NONCLIENTMETRICS ncmMetrics = { sizeof(NONCLIENTMETRICS) };
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncmMetrics, 0);
    memcpy(&lf, &ncmMetrics.lfMessageFont, sizeof lf);
}
