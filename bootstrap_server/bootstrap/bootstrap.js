const {exec} = require("child_process") ; 
 
const express = require("express");


var app = express();

app.use(express.urlencoded());

app.use(express.json());

app.post('/', (req, res) => {
    console.log( req.body );
    exec("ls -la", (error, stdout, stderr) => {
        if (error) {
            console.log(`error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.log(`stderr: ${stderr}`);
            return;
        }
        console.log(`stdout: ${stdout}`);
    });
});

app.listen(8080, () => console.log(`Started server at http://localhost:8080!`));
