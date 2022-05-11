// @ts-ignore
import mongoose, {model, Model, Schema} from "mongoose";
import {InfoNoeud} from "./Interface/InfoNoeud";

export class DatabaseAccess {

    private noeudSchema: Schema = new Schema(
        {
            nodeID: {
                port: {type: Number, required: true},
                adresseReelle: {type: String, required: true},
                adresseVirtuelle: {type: Number, required: true},
            },
            vue: [{
                nodeID: {
                    port: {type: String, required: true},
                    adresseReelle: {type: String, required: true},
                    adresseVirtuelle: {type: String, required: true},
                },
                seed: {type: Number, required: true},
                hitCount: {type: Number, required: true},
            }],
            age: {type: Number, required: false},
            malicieux: {type: Number, required: true},
        },
    );

    private noeudModel: Model<InfoNoeud> = model<InfoNoeud>('Info_Noeuds', this.noeudSchema);

    addInfo(infoNoeud: any) {
        const noeud: InfoNoeud = new this.noeudModel({
            nodeID: infoNoeud.nodeID,
            vue: infoNoeud.vue,
            malicieux: infoNoeud.malicieux,
        });
        noeud.save().then(() => console.log("Ajout réussi\n"));
    }

    recupNoeudExistant(infoNoeud: any): Promise<any> {
        return this.noeudModel.findOne({nodeID: infoNoeud.nodeID}).exec();
    }

    recupTotalNoeudSain():Promise<number>{
        return this.noeudModel.find().countDocuments({malicieux:0}).exec();
    }

    recupTotalMalicieux():Promise<number>{
        return this.noeudModel.find().countDocuments({malicieux:{$ne:0}}).exec();
    }

    recupAllNoeud():Promise<Array<InfoNoeud>>{
        return this.noeudModel.find().exec();
    }

    async updateNoeud(noeud: any) {
        const noeudEnregistrer: InfoNoeud = await this.recupNoeudExistant(noeud);
        noeudEnregistrer.vue = noeud.vue;
        noeudEnregistrer.age = noeud.age;
        noeudEnregistrer.malicieux = noeud.malicieux;
        noeudEnregistrer.save().then(() => console.log("Modification réussi\n"));
    }

    openDb() {
        const urlmongo = `mongodb+srv://Damien:EZi1eNGarEvEnzVo@test.bnuu4.mongodb.net/RéseauxData?retryWrites=true&w=majority`;
        mongoose.connect(urlmongo);
        const db = mongoose.connection;
        db.on('error', console.error.bind(console, 'Erreur lors de la connexion'));
        db.once('open', () => {
            console.log("Connexion à la base de donnée réussi");
        });
    }
}
