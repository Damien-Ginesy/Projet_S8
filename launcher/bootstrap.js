const { exec } = require("child_process");

const express = require("express");


var app = express();
app.use(express.urlencoded());

app.use(express.json());
app.use(express.static('public'));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html')
})


app.post('/bootstrap', (req, res) => {

    const option1 = req.body.survey_options1;
    const option2 = req.body.survey_options2;
    const info = req.body.info;
    const port = req.body.port;
    const ip = req.body.ip;

    var cmd = "./launcher.sh "
    cmd += "--port " + port;
    cmd += " --ip " + ip;

    for (let index = 0; index < option1.length; index++) {
        if (index % 3 == 0) {
            cmd += " --machine ";
        }
        cmd += option1[index] + " ";

    }
    if (option2[1] != "" || option2[2] != "") {
        for (let index = 0; index < option2.length; index++) {
            if (option2[index] == 'eclipse' || option2[index] == 'inst') {
                cmd += "--attack ";
            }
            cmd += option2[index] + " ";

        }
    }
    cmd += "--info ";
    cmd += "--taille_vue ";
    cmd += info[0] + " ";
    cmd += "--nb_cycle_reset ";
    cmd += info[1] + " ";
    cmd += "--nb_reset ";
    cmd += info[2] + " ";
    cmd += "--nb_cycle_sec ";
    cmd += info[3] + " ";
    cmd += "--log_server_hostname ";
    cmd += info[4] + " ";
    cmd += "--log_server_port ";
    cmd += info[5] + " ";
    console.log(cmd)



    exec(cmd, (error, stdout, stderr) => {
        if (error) {
            console.log(`error: ${error.message}`);
        }
        if (stderr) {
            console.log(`stderr: ${stderr}`);
            
        }
        console.log(`stdout: ${stdout}`);
    });
});
app.post('/stop', (req, res) => {

    var cmd = "chmod ugo+rwx ../stop_simulation.sh; ../stop_simulation.sh "
    exec(cmd, (error, stdout, stderr) => {
         if (error) {
             console.log(`error: ${error.message}`);
         }
         if (stderr) {
             console.log(`stderr: ${stderr}`);
             
         }
         console.log(`stdout: ${stdout}`);
     });
});

app.listen(8080, () => console.log(`Started server at http://@IP_of_main_machine:8080 !`));
