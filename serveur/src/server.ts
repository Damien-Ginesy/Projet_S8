// @ts-ignore
import express from 'express';
import {DatabaseAccess} from './db'
import {InfoNoeud} from "./Interface/InfoNoeud";
import * as dotenv from 'dotenv';
import {Timestamp} from "mongodb";

const app = express();
const port: number = 3000;

app.set('views', './views');
app.use(express.static(__dirname + '/../public'));
app.use('/panel', express.static(__dirname + '/../panel'));
app.use(express.json());
app.set('view engine', 'pug');

dotenv.config();
dotenv.config({ path: __dirname+'/.env' });


const db = new DatabaseAccess();

if(process.argv.length < 3){
    console.log("Merci de spécifier votre contexte d'exécution local ou online");
    process.exit(0);
}

db.openDb(process.argv[2]);

app.post('/infoNoeud', async (req, res) => {
    await db.ajoutDonnees(req.body[0]);
    res.sendStatus(200);
})

app.post('/infoNoeudVue',async (req, res) => {
    const vueNoeud = req.body.vueNoeud;
    const dataVue = await db.recupNoeudVue(vueNoeud);
    const data = {
        vueNoeudSain:dataVue[0],
        vueNoeudMalicieux:dataVue[1],
    };
    res.status(200).json(data);
})

app.get('/nodeStat', async (req, res) => {
    const noeudMalicieux = await db.recupTotalMalicieux();
    const noeudSain = await db.recupTotalNoeudSain();
    const data = {
        noeudMalicieux: noeudMalicieux,
        noeudSain:noeudSain,
    }
    res.status(200).json(data);
})


app.get('/nodeData', async (req,res)=>{
    const allNode = await db.recupAllNoeud();
    const newNodeArray = {};
    allNode.forEach(el => {
        // @ts-ignore
        newNodeArray[el.nodeID.adresseVirtuelle] = el;
    });

    res.status(200).header("Access-Control-Allow-Origin","*").json(newNodeArray);
})

app.get('/nodeDataTab', async (req,res)=>{
    const allNode = await db.recupAllNoeud();
    res.status(200).header("Access-Control-Allow-Origin","*").json(allNode);
})

app.get('/network', (req, res) => {
    res.redirect('/panel');
})

app.get('/stats', (req,res)=>{
    res.render('stats')
})

app.get('/statsData', async(req,res)=>{
    const noeudSains = await db.recupTotalNoeudSain();
    const noeudMalicieux = await db.recupTotalMalicieux();
    const nodesJson = {
        nbSains: noeudSains,
        nbMalicieux: noeudMalicieux,
    }
    // const data = JSON.stringify(nodesJson);
    res.status(200).json(nodesJson);

})


app.get('/stats/:id', async(req,res)=>{
})

app.get('/accueil', (req, res) => {
    res.render('homepage');
})

app.post('/',async (req, res) => {
    await db.ajoutDonnees(req.body[0]);
    res.sendStatus(200);
})

app.get('/', (req, res) => {
    res.redirect('/accueil');
});


app.listen(port, () => {
    console.log(`Server is listening on ${port}`);
});
