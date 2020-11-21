//
// Created by Garry Morrison on 20/11/2020.
//

#include "OperatorUsageMap.h"

OperatorUsageMap operator_usage_map;

OperatorUsageMap::OperatorUsageMap() {
    operator_usage_map.map["empty"] =
            "\nempty:\n"
            "    description:\n\n"
            "    examples:\n\n"
            "    see also:\n";

    operator_usage_map.map["clean"] =
            "\nclean:\n"
            "    description:\n"
            "        clean ket\n"
            "        clean the coefficients of the given superposition\n"
            "        if x < 0, return 0, else return 1\n\n"
            "    examples:\n"
            "        clean (3|a> + 2.2|b> - 3 |c> + |d>)\n"
            "            |a> + |b> + 0|c> + |d>\n\n"
            "        drop clean (3|a> + 2.2|b> - 3 |c> + |d>)\n"
            "            |a> + |b> + |d>\n\n"
            "    see also:\n"
            "        drop\n";

    operator_usage_map.map["drop"] =
            "\ndrop:\n"
            "    description:\n"
            "        drop sp\n"
            "        drop all kets with coeff <= 0 from the given superposition\n\n"
            "    examples:\n"
            "        drop (|a> + 0|b> - 2|c> + 7.1|d>)\n"
            "            |a> + 7.100000|d>\n\n"
            "    see also:\n"
            "        drop-below, drop-above, sdrop\n";

    operator_usage_map.map["filter"] =
            "\nfilter:\n"
            "    description:\n"
            "        filter(operators, conditions) input-seq\n"
            "        Filters the input sequence to only those elements that satisfy the operator/condition pair\n"
            "        It is more powerful than the such-that[] operator\n"
            "        Currently input-seq is converted to superposition, so sequence structure is ignored\n"
            "        Will probably change this behaviour later\n\n"
            "    examples:\n"
            "        -- learn some knowledge\n"
            "        is-food |bread> => |yes>\n"
            "        is-food |cheese> => |yes>\n"
            "        is-food |steak> => |yes>\n\n"
            "        is-furniture |chair> => |yes>\n"
            "        is-furniture |table> => |yes>\n"
            "        is-furniture |stool> => |yes>\n"
            "        is-furniture |lounge> => |yes>\n\n"
            "        is-day-of-week |monday> => |yes>\n"
            "        is-day-of-week |tuesday> => |yes>\n"
            "        is-day-of-week |wednesday> => |yes>\n"
            "        is-day-of-week |thursday> => |yes>\n"
            "        is-day-of-week |friday> => |yes>\n"
            "        is-day-of-week |saturday> => |yes>\n"
            "        is-day-of-week |sunday> => |yes>\n\n"
            "        -- now try some filters:\n"
            "        -- filter all known kets to those that are furniture:\n"
            "        filter(|op: is-furniture>, |yes>) rel-kets[*]\n"
            "            |chair> + |table> + |stool> + |lounge>\n\n"
            "        -- filter all known kets to those that are food:\n"
            "        filter(|op: is-food>, |yes>) rel-kets[*]\n"
            "            |bread> + |cheese> + |steak>\n\n"
            "        -- filter all known kets to those that are days of the week:\n"
            "        filter(|op: is-day-of-week>, |yes>) rel-kets[*]\n"
            "            |monday> + |tuesday> + |wednesday> + |thursday> + |friday> + |saturday> + |sunday>\n\n\n"
            "        -- an indirect filter example, first learn some knowledge:\n"
            "        father |John> => |Fred>\n"
            "        occupation |Fred> => |politician>\n\n"
            "        father |Sam> => |Robert>\n"
            "        occupation |Robert> => |doctor>\n\n"
            "        father |Emma> => |Jack>\n"
            "        occupation |Jack> => |nurse>\n\n"
            "        -- find people that have a father with occupation nurse:\n"
            "        -- NB: Note the ops: rather than op: to signify operator sequence rather than just an operator\n"
            "        filter(|ops: occupation father>, |nurse>) rel-kets[*]\n"
            "            |Emma>\n\n"
            "        -- find people that have a father with occupation doctor:\n"
            "        filter(|ops: occupation father>, |doctor>) rel-kets[*]\n"
            "            |Sam>\n\n\n"
            "        -- find people that have the father operator defined:\n"
            "        -- which is very close in function to the rel-kets[father] operator\n"
            "        filter(|op: father>, |*>) rel-kets[*]\n"
            "            |John> + |Sam> + |Emma>\n\n"
            "        -- filter to people that have a father with occupation of either doctor or nurse:\n"
            "        filter(|ops: occupation father>, |doctor> + |nurse>) rel-kets[*]\n"
            "            |Sam> + |Emma>\n\n"
            "        -- filter to those that have a rule of any type that is doctor or nurse:\n"
            "        -- NB: if rel-kets[*] is large, or supported-ops is large, this may be slow.\n"
            "        filter(|*>, |doctor> + |nurse>) rel-kets[*]\n"
            "            |Robert> + |Jack>\n\n\n"
            "        -- Finally, filters can be easily chained.\n"
            "        -- Eg, To find all kets that are human, American and are politicians:\n"
            "        -- NB: will be faster if you apply the most strict condition(s) first.\n"
            "        -- Eg, in this case, politician first, then American, then human.\n"
            "        filter(|op: is-human>, |yes>) filter(|op: is-american>, |yes>) filter(|op: occupation>, |politician>) rel-kets[*]\n\n"
            "    see also:\n\n"
            "    TODO:\n"
            "        preserve sequence structure of input-seq, currently it is cast to superposition\n"
            "        currently coefficients of kets are ignored, maybe they should be preserved";

    operator_usage_map.map["apply"] =
            "\napply:\n"
            "    description:\n"
            "        apply(seq, seq)\n"
            "        wrapper around apply op\n\n"
            "    examples:\n"
            "        -- learn some knowledge\n"
            "        age |Fred> => |35>\n"
            "        nick-name |Fred> => |Freddie>\n"
            "        mother |Fred> => |Jude>\n"
            "        father |Fred> => |Tom>\n"
            "        age |Jude> => |61>\n\n"
            "        -- implements: age |Fred>\n"
            "        apply(|op: age>, |Fred>)\n"
            "            |35>\n\n"
            "        -- implements: age |Fred> + mother |Fred> + father |Fred>\n"
            "        apply(|op: age> + |op: mother> + |op: father>, |Fred>)\n"
            "            |35> + |Jude> + |Tom>\n\n"
            "        -- implements: age |Fred> . mother |Fred> . father |Fred>\n"
            "        apply(|op: age> . |op: mother> . |op: father>, |Fred>)\n"
            "            |35> . |Jude> . |Tom>\n\n"
            "        -- the star operator, which adds all the right hand rules together:\n"
            "        star |*> #=> apply(supported-ops|_self>, |_self>) |>\n\n"
            "        -- Eg, applied to Fred:\n"
            "        star |Fred>\n"
            "            |35> + |Freddie> + |Jude> + |Tom>\n\n"
            "        -- using an operator sequence instead:\n"
            "        -- implements: age mother |Fred>\n"
            "        apply(|ops: age mother>, |Fred>)\n"
            "            |61>\n\n"
            "    see also:\n"
            "        learn, add-learn, seq-learn";

    operator_usage_map.map["learn"] =
            "\nlearn:\n"
            "    description:\n"
            "        learn(sp, sp, seq)\n"
            "        wrapper around a learn rule\n\n"
            "    examples:\n"
            "        -- implements: age |Fred> => |37>\n"
            "        learn(|op: age>, |Fred>, |37>)\n\n"
            "    see also:\n"
            "         add-learn, seq-learn, apply";
}

std::string OperatorUsageMap::get_usage(const std::string &s) const {
    auto it = map.find(s);
    if (it == map.end()) { return ""; }
    return it->second;
}

