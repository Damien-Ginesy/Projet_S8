// @ts-ignore
import mongoose, {model, Model, Schema} from "mongoose";
import {InfoNoeud} from "./Interface/InfoNoeud";

export class DatabaseAccess {

    private ajout = false;

    private noeudSchema: Schema = new Schema(
        {
            nodeID: {
                port: {type: Number, required: true},
                adresseReelle: {type: String, required: true},
                adresseVirtuelle: {type: Number, required: true},
            },
            vue: [{
                nodeID: {
                    port: {type: Number, required: true},
                    adresseReelle: {type: String, required: true},
                    adresseVirtuelle: {type: Number, required: true},
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
            age: infoNoeud.age,
        });
        noeud.collection.insertOne(infoNoeud).then(() => console.log("Ajout réussi"));
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

    async recupNoeudVue(vueNoeud: Array<object>): Promise<number[]> {
        const dataNoeudVue = [];
        dataNoeudVue[0] = await this.noeudModel.find().countDocuments({nodeID: {$in: vueNoeud},malicieux: 0}).exec();
        dataNoeudVue[1] = await this.noeudModel.find().countDocuments({nodeID: {$in: vueNoeud},malicieux: {$ne: 0}}).exec();
        return dataNoeudVue;
    }

    async updateNoeud(noeud: any) {
        const noeudEnregistrer: InfoNoeud | null = await this.recupNoeudExistant(noeud) || null;
        if(noeudEnregistrer !== null){
            noeudEnregistrer.vue = noeud.vue;
            noeudEnregistrer.age = noeud.age;
            noeudEnregistrer.malicieux = noeud.malicieux;
            noeudEnregistrer.save().then(() => console.log("Modification réussi"));
        }
    }

    suppDoublons(){
        this.noeudModel.distinct("nodeID").exec().then(async res => {
            for (let noeud of res) {
                const nodeID ={
                    nodeID:noeud,
                }
                const noeudExistant = await this.noeudModel.find({nodeID:nodeID.nodeID}).exec();
                let i = noeudExistant.length
                const longueurMax = noeudExistant.length;
                while(i > 1){
                    this.noeudModel.deleteOne({nodeID:noeudExistant[longueurMax-i].nodeID}).exec().then();
                    i--;
                }
            }
        })
        console.log("Doublons supprimée");
    }


    async ajoutDonnees(adresseReelle:string,noeud:object){
        // @ts-ignore
        noeud.nodeID.adresseReelle = adresseReelle;
        const existe: InfoNoeud | null = await this.recupNoeudExistant(noeud);
        if (existe === null) {
            this.addInfo(noeud);
        } else {
            await this.updateNoeud(noeud);
        }
    }

    openDb(env:string) {
        let urlmongo = "";

        if(env.toLowerCase() === 'local'){
            urlmongo = process.env.ACCESS_DB_LOCAL || "";
        }else if(env.toLowerCase() === 'online'){
            urlmongo = process.env.ACCESS_DB_ONLINE || "";
        }

        mongoose.connect(urlmongo);
        const db = mongoose.connection;

        db.on('error', () => {
            console.error.bind(console, 'Erreur lors de la connexion');
            process.exit(1);
        });
        db.once('open', () => {
            console.log("Connexion à la base de donnée réussi");
            db.dropCollection("info_noeuds");
        });

    }
}
