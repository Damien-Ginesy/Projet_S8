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

const get_free_port = () => {

    return async_exec('./find_free_port.sh');

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
app.get('/media/stop', (req, res) => {
    res.sendFile(__dirname+'/stop.png');
});
app.get('/media/metric', (req, res) => {
    res.sendFile(__dirname+'/metric.png');
});
app.get('/media/activity_indicator', (req, res) => {
    res.sendFile(__dirname+'/activity_indicator.gif');
});

// Launch
app.post('/launch', async (req, res)=>{

    stop_simu();

    let params = JSON.parse(req.body.req);

    let bootstrap_port = await get_free_port();

    // launch metric server

    exec(
        `./launch_metric_server.sh`,
        (err, stdout, stderr) => {
            if(err)
                console.log(err);
            if(stderr)
                console.log(stderr);
        }
    );

    // launch bootstrap server
    const bootstrap_server_launch = (params)=>{
        
        // total_node_number
        let total_node_number = 0;
        for(let i = 0; i < params.hosts.length; i++){
            total_node_number += parseInt(params.hosts[i].node_nbr);
        }

        let attacks_list_str = '';
        for(let i = 0; i < params.attacks.length; i++){
            attacks_list_str += ' ' + params.attacks[i].type + ' ';
            attacks_list_str += params.attacks[i].id;
            if(params.attacks[i].type == 'inst'){
                attacks_list_str +=  ' ' + params.attacks[i].mask;
            }
        }

        exec(
            `/home/Projet_S8/bootstrap_server/bin/bootstrap_server${attacks_list_str} ${total_node_number} ${bootstrap_port}`,
            (err, stdout, stderr) => {
                if(err)
                    console.log(err);
                if(stderr)
                    console.log(stderr);
            }
        );
    }

    bootstrap_server_launch(params);

    // launch basalt (ansible)

    // --- ansible : creating an inventory at /etc/ansible/hosts
    await async_exec('rm -f /etc/ansible/hosts');
    for(let i = 0; i < params.hosts.length; i++){
        await async_exec(`echo ${params.hosts[i].ip} >> /etc/ansible/hosts`);
    }

    res.json({msg : 'ok'});
});

// update code
app.post('/update', async(req, res)=>{

    stop_simu();

    let cmd_stdout;

    cmd_stdout = await async_exec('./update_code.sh');
    console.log(cmd_stdout);

    // close the launcher
    process.exit(0);

});

// stop simu

const stop_simu = async ()=>{
    
    await async_exec('./stop_simu.sh');

}

app.post('/stop', async(req, res)=>{

    stop_simu();

    res.json({msg:'ok'});

});

app.listen(port, () => console.log(`Started server at port ${port} !`));
