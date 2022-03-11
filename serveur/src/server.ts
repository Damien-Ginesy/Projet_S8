import express from 'express';
import {DatabaseAccess} from './db'
const app = express();
const port = 3000;

app.set('views','./views');
app.set('view engine','pug');

const db = new DatabaseAccess("Damien", "N3cozwDA20fg");
db.addInfo(["P8","P7","P5"]);

app.get('/acceuil',(req, res) => {
    res.render('homepage');
})

app.get('/',(req, res) =>{
    res.redirect('/acceuil');
});

app.listen(port ,() => {
    console.log(`Server is listening on ${port}`);
});