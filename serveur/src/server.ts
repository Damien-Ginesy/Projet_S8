import express from 'express';

const app = express();
const port = 3000;

app.set('views','./views');
app.set('view engine','pug');

app.get('/',(req, res) =>{
    res.render('homepage');
});

app.listen(port ,() => {
    console.log(`Server is listening on ${port}`);
});