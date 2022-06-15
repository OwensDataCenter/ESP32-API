var http = require('http');
var url = require('url');
var fs = require('fs');
http.createServer(function (req, res) {
  var q = url.parse(req.url, true);
  var filename = "." + q.pathname;
  fs.readFile(filename, function(err, data) {
    if (err) {
      res.writeHead(404, {'Content-Type': 'text/html'});
      return res.end("The specified Device ID was not found");
    } 
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.write(data);
    return res.end();
  });
}).listen(82);
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  var q = url.parse(req.url, true).query;
  var txt = q.battery + "," + q.devid + "," + q.rssi + "," + q.bootcount + "," + q.var1 + "," + q.var2 + "," + q.var3;
  var path = q.devid + ".tmp";
  fs.writeFile(path, txt, function (err) {
    if (err) throw err;
    res.end('WRITE_STATUS_OK');
  });
}).listen(81);