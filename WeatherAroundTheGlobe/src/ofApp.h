#pragma once

#include "ofMain.h"

// storing weather information
struct WeatherData {

	string city;
	string country;
	string condition;

	float temperature = 0;
	float windSpeed = 0;

	int humidity = 0;

	bool loaded = false;
};

class ofApp : public ofBaseApp {

public:
	// declaring the main functions
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);

	// declaring my own functions
	void searchWeather();
	void clearWeather();
	void loadTopCities();

	bool getWeather(
		string location,
		WeatherData & weather);

	void drawTitle();
	void drawCityList();
	void drawSearchArea();
	void drawInputBox();
	void drawButtons();

	void drawWeatherIcon(
		string condition,
		float x,
		float y);

	void drawTemperature(
		float temperature,
		float x,
		float y);

	// storing the API key
	string apiKey;

	// storing the location entered by the user
	string locationInput;

	// storing messages for the user
	string statusMessage;

	// storing the top world cities
	vector<string> topCities;

	// storing weather for the top cities
	vector<WeatherData> cityWeather;

	// storing the searched weather
	WeatherData searchResult;

	// checking if the input box is selected
	bool inputActive;

	// storing the areas of the controls
	ofRectangle inputBox;
	ofRectangle searchButton;
	ofRectangle clearButton;

	// storing the fonts
	ofTrueTypeFont titleFont;
	ofTrueTypeFont headingFont;
	ofTrueTypeFont cityFont;
	ofTrueTypeFont normalFont;
};
