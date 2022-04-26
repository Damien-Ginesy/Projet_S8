import express from 'express';
import {DatabaseAccess} from './db'
import {InfoNoeud} from "./Interface/InfoNoeud";

const app = express();
const port:number = 3000;

app.set('views','./views');
app.use(express.static(__dirname + '/../public'));
app.use(express.json());
app.set('view engine','pug');


if(process.argv.length !== 4){
    console.error("Le programme attend en argument un User et un password pour la base de donnée\n");
    process.exit();
}


const db = new DatabaseAccess(process.argv[2], process.argv[3]);

app.post('/infoNoeud',async (req, res) => {
    console.log(req.body);
    const existe: InfoNoeud | undefined = await db.recupNoeud(req.body[0]);
    if(existe === undefined){
        db.addInfo(req.body[0]);
    }
    else{
        await db.updateNoeud(req.body);
    }
    res.sendStatus(200);
})

app.get('/network', (req,res)=>{
    res.render('network');
})

app.get('/stats', (req,res)=>{
    res.render('stats');
})

app.get('/accueil',(req, res) => {
    res.render('homepage');
})

app.get('/stats',(req, res) => {
    res.render('stats');
})

app.get('/',(req, res) =>{
    res.redirect('/accueil');
});



app.listen(port ,() => {
    console.log(`Server is listening on ${port}`);
});
