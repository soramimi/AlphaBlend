#include "MainView.h"
#include <stdint.h>
#include <QPainter>
#include <QDebug>

struct RGBA8888 {
	uint8_t r, g, b, a;
	RGBA8888(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0)
		: r(r)
		, g(g)
		, b(b)
		, a(a)
	{
	}
};

static inline int div255(int v)
{
	return (v * 257 + 256) / 65536;
}

static inline RGBA8888 blend(RGBA8888 const &base, RGBA8888 const &over)
{
	if (over.a == 0) return base;
	if (base.a == 0 || over.a == 255) return over;
	int r = over.r * over.a * 255 + base.r * base.a * (255 - over.a);
	int g = over.g * over.a * 255 + base.g * base.a * (255 - over.a);
	int b = over.b * over.a * 255 + base.b * base.a * (255 - over.a);
	int a = over.a * 255 + base.a * (255 - over.a);
	return RGBA8888(r / a, g / a, b / a, div255(a));
}

MainView::MainView(QWidget *parent)
	: QWidget(parent)
{
	int w = 16;
	int h = 16;
	QImage img(w, h, QImage::Format_Grayscale8);
	for (int y = 0; y < h; y++) {
		uint8_t *p = img.scanLine(y);
		for (int x = 0; x < w; x++) {
			p[x] = ((x ^ y) & 8) ? 192 : 255;
		}
	}
	bg_brush_ = QBrush(img);
}

void MainView::paintEvent(QPaintEvent *)
{
	QPainter pr(this);
	int w = 256;
	int h = 256;
	QImage img(w, h, QImage::Format_RGBA8888);
	{
		QPainter pr(&img);
		pr.fillRect(0, 0, w, h, bg_brush_);
	}

	for (int y = 0; y < h; y++) {
		RGBA8888 *p = (RGBA8888 *)img.scanLine(y);
		for (int x = 0; x < w; x++) {
			RGBA8888 over1;
			over1.r = 255;
			over1.g = 0;
			over1.b = 0;
			over1.a = x * 255 / w;
			RGBA8888 over2;
			over2.r = 0;
			over2.g = 0;
			over2.b = 255;
			over2.a = y * 255 / w;
			p[x] = blend(p[x], blend(over1, over2));
		}
	}

	pr.drawImage(0, 0, img);
}
