#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// setting the window title
	ofSetWindowTitle("Weather Around the Globe");

	// setting the frame rate
	ofSetFrameRate(60);

	// loading the fonts
	titleFont.load("arialbd.ttf", 36);
	headingFont.load("arialbd.ttf", 18);
	cityFont.load("arialbd.ttf", 16);
	normalFont.load("arial.ttf", 14);

	// storing the WeatherAPI key
	apiKey = "a852290496ba4c009be210545261810";

	// starting with an empty location
	locationInput = "";

	// starting with a simple message
	statusMessage = "Enter a location to begin.";

	// starting the input box as selected
	inputActive = true;

	// storing the top world cities
	topCities = {
		"London",
		"Paris",
		"New York",
		"Tokyo",
		"Dubai",
		"Sydney",
		"Singapore",
		"Rome",
		"Toronto",
		"Bucharest"
	};

	// setting the position and size of the input box
	inputBox.set(180, 625, 640, 45);

	// setting the position and size of the buttons
	searchButton.set(300, 690, 160, 45);
	clearButton.set(540, 690, 160, 45);

	// loading weather for the top cities
	loadTopCities();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	// setting the main background colour
	ofBackground(243, 244, 246);

	// drawing the title
	drawTitle();

	// drawing the city list
	drawCityList();

	// drawing the search area
	drawSearchArea();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// checking if the input box is active
	if (inputActive) {

		// deleting the last character
		if (key == OF_KEY_BACKSPACE) {

			if (locationInput.length() > 0) {
				locationInput.pop_back();
			}
		}

		// searching when Enter is pressed
		else if (key == OF_KEY_RETURN) {
			searchWeather();
		}

		// adding normal keyboard characters
		else if (key >= 32 && key <= 126) {
			locationInput += (char)key;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// selecting the input box
	if (inputBox.inside(x, y)) {
		inputActive = true;
	} else {
		inputActive = false;
	}

	// checking the search button
	if (searchButton.inside(x, y)) {
		searchWeather();
	}

	// checking the clear button
	if (clearButton.inside(x, y)) {
		clearWeather();
	}
}

//--------------------------------------------------------------
void ofApp::searchWeather() {

	// checking if a location was entered
	if (locationInput == "") {
		statusMessage = "Please enter a location.";
		return;
	}

	// checking if the API key was added
	if (apiKey == "PASTE_YOUR_API_KEY_HERE") {
		statusMessage = "Please add your WeatherAPI key.";
		return;
	}

	// getting weather for the searched location
	bool found = getWeather(
		locationInput,
		searchResult);

	// checking if the location was found
	if (found) {
		statusMessage = "Weather found successfully.";
	} else {
		statusMessage = "Location not found.";
	}
}

//--------------------------------------------------------------
void ofApp::clearWeather() {

	// clearing the location
	locationInput = "";

	// clearing the searched weather
	searchResult = WeatherData();

	// clearing the weather message
	statusMessage = "Enter a location to begin.";

	// selecting the input box again
	inputActive = true;
}

//--------------------------------------------------------------
void ofApp::loadTopCities() {

	// clearing old weather information
	cityWeather.clear();

	// loading weather for every city
	for (int i = 0; i < topCities.size(); i++) {

		WeatherData weather;

		weather.city = topCities[i];

		// getting weather from the API
		getWeather(
			topCities[i],
			weather);

		// storing the weather result
		cityWeather.push_back(weather);
	}
}

//--------------------------------------------------------------
bool ofApp::getWeather(
	string location,
	WeatherData & weather) {

	// checking if the API key was added
	if (apiKey == "PASTE_YOUR_API_KEY_HERE") {
		return false;
	}

	// preparing locations containing spaces
	string preparedLocation = location;

	ofStringReplace(
		preparedLocation,
		" ",
		"%20");

	// creating the API address
	string url = "https://api.weatherapi.com/v1/current.json?key="
		+ apiKey
		+ "&q="
		+ preparedLocation;

	// requesting weather data
	ofHttpResponse response = ofLoadURL(url);

	// checking if the request worked
	if (response.status != 200) {

		weather.loaded = false;

		return false;
	}

	try {

		// converting the returned information to JSON
		ofJson weatherJson = ofJson::parse(
			response.data.getText());

		// getting the location information
		weather.city = weatherJson["location"]["name"]
						   .get<string>();

		weather.country = weatherJson["location"]["country"]
							  .get<string>();

		// getting the temperature
		weather.temperature = weatherJson["current"]["temp_c"]
								  .get<float>();

		// getting the weather condition
		weather.condition = weatherJson["current"]["condition"]["text"]
								.get<string>();

		// getting the humidity
		weather.humidity = weatherJson["current"]["humidity"]
							   .get<int>();

		// getting the wind speed
		weather.windSpeed = weatherJson["current"]["wind_mph"]
								.get<float>();

		// marking the data as loaded
		weather.loaded = true;

		return true;
	} catch (...) {

		// returning false if the data cannot be read
		weather.loaded = false;

		return false;
	}
}

//--------------------------------------------------------------
void ofApp::drawTitle() {

	// storing the header area
	float headerX = 120;
	float headerY = 25;
	float headerW = 760;
	float headerH = 85;

	// storing the title text
	string titleText = "WEATHER AROUND THE GLOBE";

	// drawing the title background
	ofSetColor(50, 76, 100);

	ofDrawRectRounded(
		headerX,
		headerY,
		headerW,
		headerH,
		18);

	// calculating the centred position
	float titleX = headerX
		+ (headerW
			  - titleFont.stringWidth(titleText))
			/ 2;

	float titleY = 80;

	// drawing the title
	ofSetColor(255);

	titleFont.drawString(
		titleText,
		titleX,
		titleY);
}

//--------------------------------------------------------------
void ofApp::drawCityList() {

	int cardWidth = 390;
	int cardHeight = 70;

	int startX = 100;
	int startY = 165;

	int gapX = 20;
	int gapY = 12;

	// storing the section title
	string sectionTitle = "TOP 10 WORLD CITIES";

	// calculating the width of both columns
	float cityAreaWidth = cardWidth * 2 + gapX;

	// centering the heading above the cards
	float sectionX = startX
		+ (cityAreaWidth
			  - headingFont.stringWidth(sectionTitle))
			/ 2;

	// drawing the section title
	ofSetColor(45, 60, 72);

	headingFont.drawString(
		sectionTitle,
		sectionX,
		145);

	// drawing all city cards
	for (int i = 0; i < topCities.size(); i++) {

		int column = i % 2;
		int row = i / 2;

		int x = startX
			+ column * (cardWidth + gapX);

		int y = startY
			+ row * (cardHeight + gapY);

		// drawing the card
		ofSetColor(230, 235, 239);

		ofDrawRectRounded(
			x,
			y,
			cardWidth,
			cardHeight,
			10);

		// drawing the city name
		ofSetColor(35, 55, 70);

		cityFont.drawString(
			topCities[i],
			x + 20,
			y + 28);

		// checking if weather was loaded
		if (
			i < cityWeather.size()
			&& cityWeather[i].loaded) {

			// drawing the temperature
			drawTemperature(
				cityWeather[i].temperature,
				x + 240,
				y + 28);

			// drawing the weather condition
			ofSetColor(90, 100, 110);

			normalFont.drawString(
				cityWeather[i].condition,
				x + 20,
				y + 55);

			// drawing the weather icon
			drawWeatherIcon(
				cityWeather[i].condition,
				x + 345,
				y + 35);
		} else {

			// showing unavailable weather
			ofSetColor(90, 100, 110);

			normalFont.drawString(
				"Weather unavailable",
				x + 20,
				y + 55);
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawSearchArea() {

	// storing the search title
	string searchTitle = "SEARCH A LOCATION";

	// centering the title above the input box
	float searchTitleX = inputBox.x
		+ (inputBox.width
			  - headingFont.stringWidth(searchTitle))
			/ 2;

	// drawing the search title
	ofSetColor(45, 60, 72);

	headingFont.drawString(
		searchTitle,
		searchTitleX,
		605);

	// drawing the input box
	drawInputBox();

	// drawing the buttons
	drawButtons();

	// drawing the information box
	ofSetColor(230, 235, 239);

	ofDrawRectRounded(
		150,
		755,
		700,
		130,
		10);

	// drawing the information title
	ofSetColor(35, 55, 70);

	cityFont.drawString(
		"Weather Information",
		175,
		785);

	// checking if searched weather exists
	if (searchResult.loaded) {

		// preparing location information
		string locationText = "Location: "
			+ searchResult.city
			+ ", "
			+ searchResult.country;

		// drawing the location
		ofSetColor(65, 75, 85);

		normalFont.drawString(
			locationText,
			175,
			815);

		// drawing the temperature label
		normalFont.drawString(
			"Temperature:",
			175,
			842);

		// drawing the temperature further from the label
		drawTemperature(
			searchResult.temperature,
			300,
			842);

		// drawing the condition
		normalFont.drawString(
			"Condition: "
				+ searchResult.condition,
			430,
			842);

		// drawing humidity
		normalFont.drawString(
			"Humidity: "
				+ ofToString(searchResult.humidity)
				+ "%",
			175,
			868);

		// drawing wind speed
		normalFont.drawString(
			"Wind: "
				+ ofToString(
					searchResult.windSpeed,
					1)
				+ " mph",
			430,
			868);

		// drawing the weather icon
		drawWeatherIcon(
			searchResult.condition,
			800,
			820);
	} else {

		// drawing the current message
		ofSetColor(65, 75, 85);

		normalFont.drawString(
			statusMessage,
			175,
			825);
	}
}

//--------------------------------------------------------------
void ofApp::drawInputBox() {

	// drawing the input background
	ofSetColor(250, 250, 250);
	ofDrawRectRounded(inputBox, 7);

	// drawing the normal border
	ofNoFill();

	ofSetColor(85, 95, 105);
	ofSetLineWidth(3);

	ofDrawRectRounded(inputBox, 7);

	// making the border stronger when selected
	if (inputActive) {

		ofSetColor(60, 110, 155);
		ofSetLineWidth(4);

		ofDrawRectRounded(inputBox, 7);
	}

	ofFill();

	// drawing the location text
	if (locationInput == "") {

		ofSetColor(125, 130, 135);

		normalFont.drawString(
			"Example: Coventry",
			195,
			655);
	} else {

		ofSetColor(30, 40, 50);

		normalFont.drawString(
			locationInput,
			195,
			655);
	}
}

//--------------------------------------------------------------
void ofApp::drawButtons() {

	// drawing the search button
	ofSetColor(115, 165, 130);

	ofDrawRectRounded(
		searchButton,
		8);

	ofSetColor(255);

	cityFont.drawString(
		"Search",
		350,
		719);

	// drawing the clear button
	ofSetColor(185, 105, 100);

	ofDrawRectRounded(
		clearButton,
		8);

	ofSetColor(255);

	cityFont.drawString(
		"Clear",
		595,
		719);
}

//--------------------------------------------------------------
void ofApp::drawTemperature(
	float temperature,
	float x,
	float y) {

	// converting the temperature to text
	string temperatureText = ofToString(
		temperature,
		1);

	// drawing the temperature number
	ofSetColor(50, 70, 85);

	normalFont.drawString(
		temperatureText,
		x,
		y);

	// finding the end of the number
	float textWidth = normalFont.stringWidth(
		temperatureText);

	// drawing the degree symbol
	ofNoFill();

	ofSetLineWidth(1.5);

	ofDrawCircle(
		x + textWidth + 5,
		y - 10,
		2.5);

	ofFill();

	// drawing the C
	normalFont.drawString(
		"C",
		x + textWidth + 11,
		y);
}

//--------------------------------------------------------------
void ofApp::drawWeatherIcon(
	string condition,
	float x,
	float y) {

	// drawing a sunny or clear icon
	if (
		condition.find("Sunny") != string::npos
		|| condition.find("Clear") != string::npos) {

		ofSetColor(235, 190, 70);

		ofDrawCircle(
			x,
			y,
			9);

		ofSetLineWidth(2);

		ofDrawLine(x, y - 16, x, y - 12);
		ofDrawLine(x, y + 12, x, y + 16);
		ofDrawLine(x - 16, y, x - 12, y);
		ofDrawLine(x + 12, y, x + 16, y);

		ofDrawLine(
			x - 11,
			y - 11,
			x - 8,
			y - 8);

		ofDrawLine(
			x + 8,
			y + 8,
			x + 11,
			y + 11);

		ofDrawLine(
			x + 8,
			y - 8,
			x + 11,
			y - 11);

		ofDrawLine(
			x - 11,
			y + 11,
			x - 8,
			y + 8);

		ofSetLineWidth(1);
	}

	// drawing a rain icon
	else if (
		condition.find("rain") != string::npos
		|| condition.find("Rain") != string::npos
		|| condition.find("drizzle") != string::npos) {

		// drawing the cloud
		ofSetColor(135, 150, 160);

		ofDrawCircle(
			x - 7,
			y,
			8);

		ofDrawCircle(
			x + 2,
			y - 5,
			10);

		ofDrawCircle(
			x + 11,
			y,
			8);

		ofDrawRectangle(
			x - 7,
			y,
			18,
			7);

		// drawing the rain
		ofSetColor(70, 130, 185);

		ofSetLineWidth(2);

		ofDrawLine(
			x - 7,
			y + 12,
			x - 10,
			y + 18);

		ofDrawLine(
			x + 2,
			y + 12,
			x - 1,
			y + 18);

		ofDrawLine(
			x + 11,
			y + 12,
			x + 8,
			y + 18);

		ofSetLineWidth(1);
	}

	// drawing a cloud icon
	else {

		ofSetColor(145, 155, 165);

		ofDrawCircle(
			x - 7,
			y,
			8);

		ofDrawCircle(
			x + 2,
			y - 5,
			10);

		ofDrawCircle(
			x + 11,
			y,
			8);

		ofDrawRectangle(
			x - 7,
			y,
			18,
			7);
	}
}
