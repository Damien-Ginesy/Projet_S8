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

// Launch
app.post('/launch', (req, res)=>{

    let params = JSON.parse(req.body.req);

    res.json({msg : 'ok'});
});


app.listen(port, () => console.log(`Started server at port ${port} !`));
