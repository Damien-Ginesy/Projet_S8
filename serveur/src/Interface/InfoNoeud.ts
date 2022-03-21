import {NodeID} from "./NodeID";
import {Vue} from "./Vue";
import {Types,Document} from "mongoose";

export interface InfoNoeud extends Document{
    nodeID:Types.DocumentArray<NodeID>,
    vue: Types.DocumentArray<Vue>,
    age: number,
    malicieux: Boolean,
}