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

// systemctl start mongod.service
exec(
    `/etc/init.d/mongodb start`,
    (err, stdout, stderr) => {
        if(err)
            console.log(err);
        if(stderr)
            console.log(stderr);

        console.log('mongo db ok');
    }
);

const async_exec = (cmd) =>{
    return new Promise((resolve)=>{
        exec(cmd,(err, stdout, stderr)=>{
            if(err){
                resolve(err);
            }else{
                resolve(stdout);
                resolve(stderr);
            }
        })
    });
}

const get_free_port = () => {

    return async_exec('./find_free_port.sh');

}

// var
var main_machain_ip = '172.17.0.15';

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
app.get('/media/scan', (req, res) => {
    res.sendFile(__dirname+'/scan.png');
});

app.post('/scan_network', async (req, res)=>{

    // nmap -n -sn 172.17.0.0/24 -oG - | awk '/Up$/{print $2}'

    let sim_net = JSON.parse(req.body.req);

    main_machain_ip = sim_net.main_mac_ip;
    console.log(`main machaine ip : ${main_machain_ip}`);

    exec(`nmap -n -sn ${sim_net.net_ip} -oG - | awk '/Up$/{print $2}'`,
        async (err, stdout, stderr) => {
            if(err)
                console.log(err);
            if(stderr)
                console.log(stderr);

            // creating inventory
            let inventory_tab = stdout.split('\n');
            inventory_tab.splice(inventory_tab.length-1);

            await async_exec("rm -f /etc/ansible/hosts");
            for(let i = 0; i < inventory_tab.length; i++){
                await async_exec(`echo ${inventory_tab[i]} >> /etc/ansible/hosts`);
            }
            console.log('inventory created');
            console.log(inventory_tab);

            // sending respond
            res.json({msg : 'ok', inventory_tab : inventory_tab});

        }
    );

});

// Launch
app.post('/launch', async (req, res)=>{

    console.log('launching');

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

            console.log('metric ok');
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
                if(stdout)
                    console.log(stdout);
                if(stderr)
                    console.log(stderr);

                res.json({msg : 'ok'});
            }
        );
    }

    bootstrap_server_launch(params);

    console.log('bootstrap ok');

    // launch basalt (ansible)

    // --- ansible : creating an inventory at /etc/ansible/hosts
    // await async_exec('rm -f /etc/ansible/hosts');
    // for(let i = 0; i < params.hosts.length; i++){
    //     await async_exec(`echo ${params.hosts[i].ip} >> /etc/ansible/hosts`);
    // }
    // await async_exec(`echo '172.17.0.13' > /etc/ansible/hosts; echo '172.17.0.14' >> /etc/ansible/hosts`);

    // --- ansible : send bin to hosts
    let res_cmd = await async_exec("su peer -c 'ansible-playbook send_bin.yaml'");
    
    // --- launch
    let launch_basalt_cmd_tab = []
    const launch_basalt_cmd_gen = async (params, mac_i, attack_id, nbr_node)=>{
        launch_basalt_cmd_tab.push(`ansible ${params.hosts[mac_i].ip} -a "/home/peer/in_host_launcher.sh ${attack_id} ${nbr_node} ${params.basalt.view_size} ${params.basalt.cycles_before_reset} ${params.basalt.nodes_per_reset} ${params.basalt.cycles_per_second} ${main_machain_ip} ${bootstrap_port}"`);
    }
    
    for(let mac_i = 0; mac_i < params.hosts.length; mac_i++){
        
        if(parseInt(params.hosts[mac_i].node_nbr) === 0) continue;
        
        for(let attack_i = 0; attack_i < params.attacks.length; attack_i++){

            if(parseInt(params.attacks[attack_i].node_nbr) === 0) continue;

            if(parseInt(params.hosts[mac_i].node_nbr) >= parseInt(params.attacks[attack_i].node_nbr)){
                launch_basalt_cmd_gen(params, mac_i, params.attacks[attack_i].id, params.attacks[attack_i].node_nbr);
                params.hosts[mac_i].node_nbr = parseInt(params.hosts[mac_i].node_nbr) - parseInt(params.attacks[attack_i].node_nbr);
                params.attacks[attack_i].node_nbr = 0;
            }else{
                launch_basalt_cmd_gen(params, mac_i, params.attacks[attack_i].id, params.hosts[mac_i].node_nbr);
                params.attacks[attack_i].node_nbr = parseInt(params.attacks[attack_i].node_nbr) - params.hosts[mac_i].node_nbr;
                params.hosts[mac_i].node_nbr = 0;
                break;
            }

        }

        if(parseInt(params.hosts[mac_i].node_nbr) > 0){
            launch_basalt_cmd_gen(params, mac_i, 0, params.hosts[mac_i].node_nbr);
        }

    }

    let main_basalt_exec_cmd = "su peer -c '";
    for(let i = 0; i < launch_basalt_cmd_tab.length; i++){
        main_basalt_exec_cmd += launch_basalt_cmd_tab[i]+" & ";
    }
    main_basalt_exec_cmd += "';";
    // console.log(main_basalt_exec_cmd);
    await async_exec(main_basalt_exec_cmd);

    console.log('basalt ok');

    // send res when bootstrap finished
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

// diagnostic
app.get('/log', (req, res)=>{
    if(req.query.log_file_name === 'inventory'){
        res.sendFile(`/etc/ansible/hosts`);
    }else{
        res.sendFile(`/home/log/${req.query.log_file_name}`);
    }
});

app.listen(port, () => console.log(`Started server at port ${port} !`));
