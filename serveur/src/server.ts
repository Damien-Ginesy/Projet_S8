import express from 'express';
import {DatabaseAccess} from './db'

const app = express();
const port = 3000;

app.set('views','./views');
app.use(express.static(__dirname + '/../public'));
app.use(express.json());
app.set('view engine','pug');


if(process.argv.length !== 4){
    console.error("Le programme attend en argument un User et un password pour la base de donnée\n");
    process.exit();
}


const db = new DatabaseAccess(process.argv[2], process.argv[3]);

app.post('/infoNoeud',(req,res)=>{
    db.addInfo(req.body);
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
