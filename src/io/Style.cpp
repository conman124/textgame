#include "Style.h"

#include <iostream>

Style::Style(Color frontColorIn, Brightness frontBrightnessIn, Color backColorIn, Brightness backBrightnessIn)
	: frontColor{frontColorIn}
	, frontBrightness{frontBrightnessIn}
	, backColor{backColorIn}
	, backBrightness{backBrightnessIn}
{

}

void Style::testStyles() {
	for(int i = 0; i < 2; ++i) {
		for(int j = 0; j < 9; ++j ) {
			for(int k = 0; k < 2; ++k) {
				for(int l = 0; l < 9; ++l) {
					Style style{static_cast<Color>(l), static_cast<Brightness>(k), static_cast<Color>(j), static_cast<Brightness>(i)};
					std::cout << style("*");
				}
			}
			std::cout << "\n";
		}
	}
}

int Style::front() const {
	return code(frontColor, frontBrightness, true);
}

int Style::back() const {
	return code(backColor, backBrightness, false);
}

int Style::code(Color color, Brightness brightness, bool isFront) const {
	int ret = 30;
	ret += static_cast<int>(color);
	ret += (brightness == Brightness::Normal) ? 0 : 60;
	ret += isFront ? 0 : 10;
	return ret;
}

const Style Style::unstyled{Color::Default};
const Style Style::red{Color::Red};
const Style Style::green{Color::Green};
const Style Style::yellow{Color::Yellow};
const Style Style::blue{Color::Blue};
const Style Style::magenta{Color::Magenta};
const Style Style::cyan{Color::Cyan};