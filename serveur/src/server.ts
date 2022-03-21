import express from 'express';
import {DatabaseAccess} from './db'
const app = express();
const fs = require('fs')
const port = 3000;

app.set('views','./views');
app.set('view engine','pug');

if(process.argv.length !== 4){
    console.error("Le programme attend en argument un User et un password pour la base de donnée\n");
    process.exit();
}

const fichier = fs.readFileSync('noeud.json');

const db = new DatabaseAccess(process.argv[2], process.argv[3]);
db.addInfo(fichier);

app.get('/accueil',(req, res) => {
    res.render('homepage');
})

app.get('/',(req, res) =>{
    res.redirect('/accueil');
});

app.listen(port ,() => {
    console.log(`Server is listening on ${port}`);
});