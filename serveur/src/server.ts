// @ts-ignore
import express from 'express';
import {DatabaseAccess} from './db'
import {InfoNoeud} from "./Interface/InfoNoeud";

const app = express();
const port: number = 3000;

app.set('views', './views');
app.use(express.static(__dirname + '/../public'));
app.use(express.json());
app.set('view engine', 'pug');

const db = new DatabaseAccess();
db.openDb();

app.post('/infoNoeud', async (req, res) => {

    let noeud: any;
    for (noeud of req.body) {
        const existe: InfoNoeud | null = await db.recupNoeudExistant(noeud);
        if (existe === null) {
            db.addInfo(noeud);
        } else {
            await db.updateNoeud(noeud);
        }
    }
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
    const nodesJson = {
        allNode: allNode,
    }
    res.status(200).header("Access-Control-Allow-Origin","*").json(nodesJson);
})

app.get('/network', (req, res) => {
    res.render('network');
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
    let noeud: any;
    for (noeud of req.body) {
        const existe: InfoNoeud | null = await db.recupNoeudExistant(noeud);
        if (existe === null) {
            db.addInfo(noeud);
        } else {
            await db.updateNoeud(noeud);
        }
    }
    res.sendStatus(200);
})

app.get('/', (req, res) => {
    res.redirect('/accueil');
});


app.listen(port, () => {
    console.log(`Server is listening on ${port}`);
});
