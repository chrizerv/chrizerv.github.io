var json_info; //all the information about the weather in json format

//Toogle between farenheit and celcius when the user pushes the buttom
function toogleCentigrate(cgrade) {
    "use strict";
    if (cgrade === 'f') {
        $("#centigrade").html(Math.floor((json_info.main.temp) * 9 / 5 - 459.67) + " &#8457;");
        $("#flip").html("To &#8451;");
        $("#flip").attr("onclick","toogleCentigrate('c');");
    }
    
    if (cgrade === 'c') {
        $("#centigrade").html(Math.floor(json_info.main.temp - 273.15) + " &#8451;" );
        $("#flip").html("To &#8457;");
        $("#flip").attr("onclick","toogleCentigrate('f');");
    }
    
}

//show weather on screen based on json_info information
function adjustWeather() {
    "use strict";
    $("#location").html(json_info.name + "," + json_info.sys.country);
    $("#centigrade").html(Math.floor(json_info.main.temp - 273.15) + " &#8451;" );
    $("#desc").html(json_info.weather[0].description);
    $("#icon").attr( "src", "http://openweathermap.org/img/w/" + json_info.weather[0].icon + ".png");
    $("body").css("background-image","url('images/" + json_info.weather[0].main +  ".jpg')");
}

function getWeatherFromCords(cords) {
    "use strict";
    //get information about weather in json format based on cords
    $.getJSON("http://api.openweathermap.org/data/2.5/weather?lat=" + cords[0] + "&lon=" + cords[1] + "&APPID=a4fc00e8d85c0833e8b309e960ab8aa2", function (json) {
        json_info = json;
        //proceed to the weather adjustment
        adjustWeather();
    });
}

//Get Coords of the client
function getGeoLocation() {
    "use strict";
    var cords = [];
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(function (position) {
            cords.push(position.coords.latitude);
            cords.push(position.coords.longitude);
            //When you take them, use them to find the weather
            getWeatherFromCords(cords);
        });
    }
}

