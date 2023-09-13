const http = require('http');
const fs = require('fs');
const path_prefix = 'data';

const PORT=3001

const reqListener = (req,res) => {
	const path =req.url;
	const realFile = path_prefix + path;
	if(fs.existsSync(realFile)) {
		const content = fs.readFileSync(realFile);
		res.end(content);
	} else {
	const data ="ciap";
	res.end(data);
	}
}

const serve = http.createServer(reqListener);

serve.listen(PORT, 'localhost');

console.log("hi world");
