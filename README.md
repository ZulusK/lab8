[![Travis-CI][travis-badge]][travis-builds]
[travis-badge]: https://travis-ci.org/ZulusK/lab8.svg?branch=master
[travis-builds]: https://travis-ci.org/ZulusK/lab8/builds
# LAB8  HTTP server

![Cat's server]( https://github.com/ZulusK/lab8/blob/master/res/cat.jpg "Cat's server")

#### Goal:
Learn how to create a simple web-server interaction from over HTTP. Learn to execute remote Git repository with the code, check the build project using continuous integration and code to create documentation project.

#### 1. Development of web-server

Create a TCP-server that listens TCP connection requests of customers and interact with them via the protocol HTTP. The server must only handle HTTP GET requests and return correctly-formed HTTP-response. Interface Server allows you to access it on the HTTP-ways:

* `/` - send in response JSON-object with information about the server:
```
{
    "Title": "% SERVER_NAME%",
    "Developer": "% YOUR_NAME_SURNAME%",
    "Time": "% SERVER_TIME%"
}
```

* ``/ Favorites`` - JSON-list of objects that represent a list of your favorite countries. Each facility must have several fields, among which must be numeric field with a unique id for each object value.

* ``/ Favorites? {Key} = {value}`` - JSON-pidspysok those objects from the list / favorites, whose field {key} matter {value}. Key {key}, in which the filtering list, choose randomly.

* ``/ Favorites / {id}`` - JSON-object in the list / favorites for numerical identifier value ``{id}``. If an object id values have not been found to return in response to HTTP Status 404 Not Found.

* ``/ File`` - read the contents of the file data.txt, which previously placed in the directory data / project. Send client JSON-object that contains the name, size (in bytes) and text file content.

* ``/ File / data`` - in response to the client to send JSON-object with information about the number of digits in the file and the largest number (file data.txt).

#### 2. Create project documentation using Doxygen


#### 3. Task №3. Continuous integration (Continuous Integration)
