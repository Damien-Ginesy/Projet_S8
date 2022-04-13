"use strict";
exports.__esModule = true;
var express_1 = require("express");
var db_1 = require("./db");
var app = (0, express_1["default"])();
var port = 3000;
app.set('views', './views');
app.set('view engine', 'pug');
var db = new db_1.DatabaseAccess("user", "password");
db.addInfo(["P8", "P7", "P5"]);
app.get('/acceuil', function (req, res) {
    res.render('homepage');
});
app.get('/', function (req, res) {
    res.redirect('/acceuil');
});
app.listen(port, function () {
    console.log("Server is listening on ".concat(port));
});
