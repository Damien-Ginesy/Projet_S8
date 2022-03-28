import {Schema, model, Model} from "mongoose";
import mongoose from "mongoose";
import {InfoNoeud} from "./Interface/InfoNoeud";

 export class DatabaseAccess {

    private noeudSchema:Schema = new Schema(
    {
        nodeID:{
            port:{type:String, required:true},
            adresseReelle:{type:String, required:true},
            adresseVirtuelle:{type:String, required:true},
        },
        vue:[{
            nodeID:{
                port:{type:String, required:true},
                adresseReelle:{type:String, required:true},
                adresseVirtuelle:{type:String, required:true},
            },
            seed:{type:Number, required: true},
            hitCount:{type:Number, required:true},
        }],
        age:{type:Number, required:true},
        malicieux:{type:Boolean, required:true},
    },
    );

    private noeudModel:Model<InfoNoeud> = model<InfoNoeud>('Info_Noeuds', this.noeudSchema);

    constructor(user:string, password:string) {
        const urlmongo = `mongodb+srv://${user}:${password}@test.bnuu4.mongodb.net/RéseauxData?retryWrites=true&w=majority`;
        this.openDb(urlmongo);
    }

    addInfo(infoNoeud:any){
        const noeud:InfoNoeud = new this.noeudModel({
            nodeID:infoNoeud.nodeID,
            vue:infoNoeud.vue,
            age:infoNoeud.age,
            malicieux: infoNoeud.malicieux,
        
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
