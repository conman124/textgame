#ifndef STYLE_H
#define STYLE_H

#include <string>
#include <fmt/core.h>

enum class Color {
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	Default
};

enum class Brightness {
	Normal,
	Bright
};

class Style {
public:
	Style(Color frontColor = Color::Default, Brightness frontBrightness = Brightness::Normal, Color backColor = Color::Default, Brightness backBrightness = Brightness::Normal);

	template <class T>
	std::string operator()(T&& input) const {
		std::string style = "\033[";
		if(frontColor != Color::Default) {
			style += std::to_string(front());
			if(backColor != Color::Default) {
				style += ";";
			}
		}
		if(backColor != Color::Default) {
			style += std::to_string(back());
		}
		return fmt::format("{}m{}\033[0m", style, input);
	}

	static void testStyles();

private:
	int front() const;

	int back() const;

	int code(Color color, Brightness brightness, bool isFront) const;

	Color frontColor;
	Brightness frontBrightness;
	Color backColor;
	Brightness backBrightness;

public:
	static const Style unstyled;
	static const Style red;
	static const Style green;
	static const Style yellow;
	static const Style blue;
	static const Style magenta;
	static const Style cyan;
};

#endif