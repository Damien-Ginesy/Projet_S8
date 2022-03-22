import {NodeID} from "./NodeID";
import {Types} from "mongoose"

export interface Vue {
    Node: Types.DocumentArray<NodeID>,
    seed:number,
    hitCount:number,
}