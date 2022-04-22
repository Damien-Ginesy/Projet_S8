const {exec} = require("child_process") ; 
 
const express = require("express");


var app = express();

app.use(express.urlencoded());

app.use(express.json());

app.post('/', (req, res) => {
    const option1 = req.body.survey_options1;
    const option2 = req.body.survey_options2;
    const info = req.body.info;
    const port = req.body.port;

    var cmd = "Ansible_command "
    cmd+= "--port " + port;

for (let index = 0; index < option1.length; index++) {
    if ( index%2==0 )
    {
     cmd += " --machine ";
    }
    cmd += option1[index]+" ";
    
}
for (let index = 0; index < option2.length; index++) {
    if ( index%3==0 )
    {
     cmd += "--machine_malicious ";
    }
    cmd += option2[index]+" ";
    
}
cmd+= "--info " ;
for (let index = 0; index < info.length; index++) {

    cmd += info[index]+" ";
    
}
console.log(cmd)

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

app.listen(8080, () => console.log(`Started server at http://localhost:8080 !`));
