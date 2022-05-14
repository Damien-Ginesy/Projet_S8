const express = require("express");
var app = express();
let port = 8080;

const { exec } = require('child_process');

const bodyParser = require("body-parser");
app.use(bodyParser.urlencoded({ extended: false }));

const cors = require("cors");
const corsOptions = {
  origin: '*',
  credentials: true,            //access-control-allow-credentials:true
  optionSuccessStatus: 200,
}
app.use(cors(corsOptions)) // Use this after the variable declaration

const async_exec = (cmd) =>{
    return new Promise((resolve)=>{
        exec(cmd,(err, stdout, stderr)=>{
            if(err){
                resolve(err);
            }else{
                resolve(stdout);
            }
        })
    });
}

const get_free_port = async () => {

    return await async_exec('./find_free_port');

}

//media
app.get('/', (req, res) => {
    res.sendFile(__dirname+'/index.html');
});
app.get('/media/logo', (req, res) => {
    res.sendFile(__dirname+'/project_logo.png');
});
app.get('/media/add', (req, res) => {
    res.sendFile(__dirname+'/add.png');
});
app.get('/media/delete', (req, res) => {
    res.sendFile(__dirname+'/delete.png');
});
app.get('/media/launch', (req, res) => {
    res.sendFile(__dirname+'/launch.png');
});
app.get('/media/refresh', (req, res) => {
    res.sendFile(__dirname+'/refresh.png');
});
app.get('/media/activity_indicator', (req, res) => {
    res.sendFile(__dirname+'/activity_indicator.gif');
});

// Launch
app.post('/launch', (req, res)=>{

    let params = JSON.parse(req.body.req);

    res.json({msg : 'ok'});
});

// update code
app.post('/update', async(req, res)=>{

    let cmd_stdout;

    cmd_stdout = await async_exec('./update_code.sh');
    console.log(cmd_stdout);

    // close the launcher
    process.exit(0);

});

app.listen(port, () => console.log(`Started server at port ${port} !`));
