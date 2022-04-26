import {Schema, model, Model} from "mongoose";
import mongoose from "mongoose";
import {InfoNoeud} from "./Interface/InfoNoeud";

 export class DatabaseAccess {

    private noeudSchema:Schema = new Schema(
    {
        nodeID:{
            port:{type:Number, required:true},
            adresseReelle:{type:String, required:true},
            adresseVirtuelle:{type:Number, required:true},
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
        age:{type:Number, required:false},
        malicieux:{type:Number, required:false},
    },
    );

    private noeudModel:Model<InfoNoeud> = model<InfoNoeud>('Info_Noeuds', this.noeudSchema);

    constructor(user:string, password:string) {
        const urlmongo = `mongodb+srv://${user}:${password}@test.bnuu4.mongodb.net/RéseauxData?retryWrites=true&w=majority`;
        /*const urlmongo = 'mongodb+srv://thomas:9jqtaEAfBloSTSam@cluster0.tr1ew.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';*/
        this.openDb(urlmongo);
    }

    addInfo(infoNoeud:InfoNoeud){
        const noeud:InfoNoeud = new this.noeudModel({
            nodeID:infoNoeud.nodeID,
            vue:infoNoeud.vue,
            malicieux: infoNoeud.malicieux,
        });
        noeud.save();
    }

    recupNoeud(infoNoeud:InfoNoeud):Promise<any>{
        return this.noeudModel.findOne({noeudID:infoNoeud.nodeID}).exec();
    }

    async updateNoeud(noeud: InfoNoeud) {
        const noeudEnregistrer:InfoNoeud = await this.recupNoeud(noeud);
        noeudEnregistrer.vue = noeud.vue;
        noeudEnregistrer.age = noeud.age;
        noeudEnregistrer.malicieux = noeud.malicieux;
        noeudEnregistrer.save();
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
