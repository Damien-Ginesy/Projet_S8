import {Schema, model, Model, Document} from "mongoose";
import mongoose from "mongoose";


interface InfoNoeuds extends Document{
    vue: Array<string>;
}

 export class DatabaseAccess {

    private noeudSchema:Schema = new Schema({
        vue: [{type: String, required: true}]
    });

    private noeudModel:Model<InfoNoeuds> = model<InfoNoeuds>('Info_Noeuds', this.noeudSchema);

    constructor(user:string, password:string) {
        const urlmongo = `mongodb+srv://${user}:${password}@test.bnuu4.mongodb.net/RéseauxData?retryWrites=true&w=majority`;
        /*const urlmongo = 'mongodb+srv://thomas:9jqtaEAfBloSTSam@cluster0.tr1ew.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';*/
        this.openDb(urlmongo);
    }

    addInfo(vue:Array<string>){
        const noeud:InfoNoeuds = new this.noeudModel({
            vue:vue
        });
        noeud.save();
    }

    private openDb(url: string) {
        mongoose.connect(url);
        const db = mongoose.connection;
        db.on('error', console.error.bind(console, 'Erreur lors de la connexion'));
        db.once('open', () => {
            console.log("Connexion à la base de donnée réussi");
        });
    }
}
