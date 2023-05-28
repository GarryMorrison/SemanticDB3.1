#include <iostream>
#include <memory>

#include "SDB3_header.h"

// FIXME: this variable needs to be defined before it links to sdb_core properly
unsigned int default_decimal_places;

int main() {
    // Test KetMap:
    // KetMap ket_map; // make it global, so don't need to define it here.
    ulong i,j,k,l;
    i = ket_map.get_idx("first");
    j = ket_map.get_idx("second");
    k = ket_map.get_idx("third");
    l = ket_map.get_idx("a: b: c: d: e");
    ket_map.print();
    // std::cout << ket_map.get_str(j) << std::endl;
    // std::cout << ket_map.get_str(l) << std::endl;

    // Test ket:
    Ket x("cat: YoYo", 3.2);
    std::cout << "label: " << x.label() << std::endl;
    std::cout << "value: " << x.value() << std::endl;
    std::cout << x.to_string() << std::endl;
    x.multiply(2);
    std::cout << x.to_string() << std::endl;

    // Test Superposition:
    Superposition sp(x);
    std::cout << sp.to_string() << std::endl;
    sp.add("alpha");
    sp.add("");
    sp.add("beta", 1.7);
    sp.add("alpha");
    std::cout << sp.to_string() << std::endl;
    std::cout << "0: " << sp.get(0).to_string() << std::endl;
    std::cout << "1: " << sp.get(1).to_string() << std::endl;
    std::cout << "2: " << sp.get(2).to_string() << std::endl;
    std::cout << "3: " << sp.get(3).to_string() << std::endl;
    std::cout << "4: " << sp.get(4).to_string() << std::endl;
    std::cout << "5: " << sp.get(5).to_string() << std::endl;
    Ket r1("one", 1), r2("two", 2), r3("three", 3), r4("four", 4);

    // Test SuperpositionIter:
    Superposition sp2;
    sp2 = r1 + r2 + r3 + r4;
    std::cout << sp2.to_string() << std::endl;
    // BUG to fix:
    // If we iterate over sp2, then it bugs out if we learn it:
    // context.learn("op", "sp", &sp2)
    //
    for (const auto k: sp2) {
        std::cout << "ket: " << k.to_string() << std::endl;
    }
    // for (const auto k: sp2) {
    //     std::cout << "ket: " << k.to_string() << std::endl;
    // }
    // return 0;

    // Test Sequence:
    Sequence seq(sp);
    seq.append(x);
    seq.append(sp2);
    std::cout << seq.to_string() << std::endl;

    // Test Sequence iteration:
    for (const auto k: seq) {
        std::cout << "sp: " << k.to_string() << std::endl;
    }

    // Test type function:
    std::cout << "ket type: " << x.type() << std::endl;
    std::cout << "sp type: " << sp.type() << std::endl;
    std::cout << "seq type: " << seq.type() << std::endl;

    // Test size function:
    std::cout << "ket size: " << x.size() << std::endl;
    std::cout << "sp size: " << sp.size() << std::endl;
    std::cout << "seq size: " << seq.size() << std::endl;

    // Test frame:
    Frame a_frame;
    // a_frame.learn(2, &x);  // Raw pointer version, now unsupported.
    // a_frame.learn(3, &sp);
    // a_frame.learn(4, &seq);
    std::shared_ptr<BaseSequence> BSx = std::make_shared<Ket>(x);
    std::shared_ptr<BaseSequence> BSsp = std::make_shared<Superposition>(sp);
    std::shared_ptr<BaseSequence> BSseq = std::make_shared<Sequence>(seq);
    a_frame.learn(2, BSx);
    a_frame.learn(2, BSsp);
    a_frame.learn(2, BSseq);
    a_frame.print();

    // Test NewContext:
    NewContext context("testing ... ");
    context.learn("age", "Fred", "37");
    context.learn("father", "Fred", "Tom");
    context.learn("father", "Tom", "Robert");
    context.learn("age", "Tom", "62");
    context.learn("wife", "Tom", "Mary");
    context.learn("wife", "Fred", "Liz");

    // context.learn(1, 2, &seq);
    // context.learn("op", "sp", &sp2);
    std::shared_ptr<BaseSequence> bseq = std::make_shared<Sequence>(seq);
    std::shared_ptr<BaseSequence> bsp2 = std::make_shared<Superposition>(sp2);
    context.learn(2, 3, bseq);
    context.learn("op", "sp", bsp2);
    context.print_universe();

    std::cout << "Age Tom: " << context.recall("age", "Tom")->to_string() << std::endl;

    // Test Superposition addition:
    Superposition sp3;
    sp3 = sp2 + sp2 + sp2;
    std::cout << "3 * sp2: " << sp3.to_string() << std::endl;

    // Test context.add_learn():
    context.learn("friend", "Sam", "Tom");
    context.add_learn("friend", "Sam", "Rob");
    context.add_learn("friend", "Sam", "Emma");

    // Test context.seq_learn():
    context.learn("friend-list", "Sam", "Tom");
    context.seq_learn("friend-list", "Sam", "Rob");
    context.seq_learn("friend-list", "Sam", "Emma");
    context.print_universe();

    // Test ContextList:
    ContextList some_context("starting context ... ");
    some_context.learn("one", "two", "three");
    some_context.add_learn("one", "two", "four");
    some_context.seq_learn("one", "two", "five");
    some_context.set("next context ... ");
    some_context.set("final context ... ");
    some_context.print_multiverse();
    some_context.set("starting context ... ");
    std::cout << "one |two>: " << some_context.recall("one", "two")->to_string() << std::endl;

    // Test SimpleOperator:
    SimpleOperator op("foo");
    std::cout << "simple operator: " << op.to_string() << std::endl;

    // Test NumericOperator:
    NumericOperator nop1(3), nop2(5.2);
    std::cout << "numeric operators: " << nop1.to_string() << ", " << nop2.to_string() << std::endl;

    // Test simple compile:
    ContextList context2("compile test ... ");
    context2.learn("next", "one", "two");
    context2.learn("next", "two", "three");
    context2.learn("next", "three","four");
    Sequence seq2("one");
    SimpleOperator op2("next");
    NumericOperator nop(7);
    Sequence result2;
    // result2 = op2.Compile(context2, seq2);
    // result2 = nop.Compile(context2, result2);
    // result2 = op2.Compile(context2, result2);
    // PoweredOperator pop1(&op2, 3), pop2(&nop, 2);  // Raw pointer version
    std::shared_ptr<BaseOperator> BO_op2 = std::make_shared<SimpleOperator>("next");
    std::shared_ptr<BaseOperator> BO_nop = std::make_shared<NumericOperator>(5);
    PoweredOperator pop1(BO_op2, 3), pop2(BO_nop, 2);
    result2 = pop1.Compile(context2, seq2);
    result2 = pop2.Compile(context2, result2);
    std::cout << "pop1: " << pop1.to_string() << std::endl;
    std::cout << "pop2: " << pop2.to_string() << std::endl;
    std::cout << "Compile result: " << result2.to_string() << std::endl;
    OperatorSequence ops;
    // ops.append(&op2);  // Raw pointer version
    // ops.append(&pop2);
    // ops.append(&op2);
    std::shared_ptr<BaseOperator> BO_pop2 = std::make_shared<PoweredOperator>(BO_nop, 3);
    ops.append(BO_op2);
    ops.append(BO_pop2);
    ops.append(BO_op2);
    std::cout << "Op Sequence: " << ops.to_string() << std::endl;
    result2 = ops.Compile(context2, seq2);
    std::cout << "Compile result: " << result2.to_string() << std::endl;
    // return 0;

    // test Superposition::apply_sigmoid(sigmoid):
    Superposition dsp1;
    dsp1.add("a");
    dsp1.add("b", - 2.2);
    dsp1.add("c", 0);
    dsp1.add("d", 3);
    std::cout << "dsp1: " << dsp1.to_string() << std::endl;
    std::cout << "clean (|a> - 2.2|b> + 0|c> + 3|d>): " << dsp1.apply_sigmoid(clean).to_string() << std::endl;
    // std::cout << "threshold-filter[3] rank split |a b c d e f>: " << rank(split(split_k)).apply_sigmoid(threshold_filter, 3).to_string() << std::endl;
    // std::cout << "sigmoid_in_range[3,5] rank split |a b c d e f>: " << rank(split(split_k)).apply_sigmoid(sigmoid_in_range, 3, 5).to_string() << std::endl;

    // Test Sequence::apply_sigmoid(sigmoid):
    Sequence dseq(dsp1);
    dseq.append(dsp1);
    dseq.append(dsp1);
    std::cout << "dseq: " << dseq.to_string() << std::endl;
    std::cout << "clean dseq: " << dseq.apply_sigmoid(clean).to_string() << std::endl;

    // Test FunctionOperatorMap:
    fn_map.print();

    // Test simple operator compile with fn_map in the background:
    Sequence seq3(dsp1);
    SimpleOperator op3("clean"), op4("shuffle"), op5("sdrop");
    std::cout << "seq3: " << seq3.to_string() << std::endl;
    std::cout << "clean seq3: " << op3.Compile(context2, seq3).to_string() << std::endl;
    std::cout << "shuffle seq3: " << op4.Compile(context2, seq3).to_string() << std::endl;
    std::cout << "sdrop seq3: " << op5.Compile(context2, seq3).to_string() << std::endl;

    // Test BracketOperator:
    /* Raw pointer version:
    SimpleOperator op_clean("clean"), op_sdrop("sdrop");
    NumericOperator nop_1(2);
    OperatorSequence op_seq1(SMINUS, &nop_1), op_seq2(SSEQ, &op_sdrop);
    BracketOperator b_op(op_seq1);
    b_op.append(op_seq2);
    std::cout << "bracket operator: " << b_op.to_string() << std::endl;
    std::cout << "Compiled bracket operator: " << b_op.Compile(context2, seq3).to_string() << std::endl;
    PoweredOperator p_op(&b_op, 7);  // Raw pointer version
    std::cout << "powered bracket operator: " << p_op.to_string() << std::endl;
    */
    // Shared pointer version:
    std::shared_ptr<BaseOperator> BO_clean = std::make_shared<SimpleOperator>("clean");
    std::shared_ptr<BaseOperator> BO_sdrop = std::make_shared<SimpleOperator>("sdrop");
    std::shared_ptr<BaseOperator> BO_nop1 = std::make_shared<NumericOperator>(2);
    OperatorSequence op_seq1(SMINUS, BO_nop1), op_seq2(SSEQ, BO_sdrop);
    BracketOperator b_op(op_seq1);
    b_op.append(op_seq2);
    // std::shared_ptr<BaseOperator> BO_b_op = std::make_shared<BracketOperator>(op_seq1);
    // BO_b_op->append(op_seq2);
    std::shared_ptr<BaseOperator> BO_b_op = std::make_shared<BracketOperator>(b_op);
    std::cout << "bracket operator: " << BO_b_op->to_string() << std::endl;
    std::cout << "pre compiled sequence: " << seq3.to_string() << std::endl;
    std::cout << "Compiled bracket operator: " << BO_b_op->Compile(context2, seq3).to_string() << std::endl;
    PoweredOperator p_op(BO_b_op, 7);
    std::cout << "powered bracket operator: " << p_op.to_string() << std::endl;


    // Test OperatorWithSequence:
    std::shared_ptr<BaseSequence> BS_seq = std::make_shared<Sequence>(seq3);
    OperatorWithSequence OpWithSeq(SMINUS, BO_b_op, BS_seq);
    std::cout << "OpWithSeq: " << OpWithSeq.to_string() << std::endl;
    std::cout << "Compiled OpWithSeq: " << OpWithSeq.Compile(context2).to_string() << std::endl;

    // Test multiple OperatorWithSequence:
    ContextList context3("Testing OperatorWithSequence ... ");
    context3.learn("op1", "k1", "op1: k1");
    context3.learn("op2", "k2", "op2: k2");
    context3.learn("op3", "k3", "op3: k3");
    context3.learn("op4", "k4", "op4: k4");
    std::shared_ptr<BaseSequence> BS_k1 = std::make_shared<Ket>("k1");
    std::shared_ptr<BaseSequence> BS_k2 = std::make_shared<Ket>("k2");
    std::shared_ptr<BaseSequence> BS_k3 = std::make_shared<Ket>("k3");
    std::shared_ptr<BaseSequence> BS_k4 = std::make_shared<Ket>("k4");
    std::shared_ptr<BaseOperator> BO_op_1 = std::make_shared<SimpleOperator>("op1");
    std::shared_ptr<BaseOperator> BO_op_2 = std::make_shared<SimpleOperator>("op2");
    std::shared_ptr<BaseOperator> BO_op_3 = std::make_shared<SimpleOperator>("op3");
    std::shared_ptr<BaseOperator> BO_op_4 = std::make_shared<SimpleOperator>("op4");
    OperatorWithSequence example_OpWithSeq(SPLUS, BO_op_1, BS_k1);
    example_OpWithSeq.append(SMINUS, BO_op_2, BS_k2);
    example_OpWithSeq.append(SSEQ, BO_op_3, BS_k3);
    example_OpWithSeq.append(SMERGE2, BO_op_4, BS_k4);
    std::cout << "example_OpWithSeq: " << example_OpWithSeq.to_string() << std::endl;
    std::cout << "example_OpWithSeq.size(): " << example_OpWithSeq.size() << std::endl;
    std::cout << "Compiled example_OpWithSeq: " << example_OpWithSeq.Compile(context3).to_string() << std::endl;
    // return 0;

    // Test Sequence.add(ket), with sign -1:
    // Turns out it was a Superposition::to_string() bug.
    /*
    Sequence test_seq1("test ket"), test_seq2("initial ket");
    test_seq1.multiply(-1);
    test_seq2.add(test_seq1);
    std::cout << "test_seq2: " << test_seq2.to_string() << std::endl;
    */

    // Test ContextList.stored_learn():
    context3.stored_learn("op4", "k4", "op4: k4");
    context3.print_universe();

    // Test ContextList.recall_type():
    std::cout << "op3 |k2> type: " << context3.recall_type("op3", "k2") << std::endl;
    std::cout << "op3 |k3> type: " << context3.recall_type("op3", "k3") << std::endl;
    std::cout << "op4 |k4> type: " << context3.recall_type("op4", "k4") << std::endl;

    // Test ContextList.learn() with OperatorWithSequence:
    std::shared_ptr<BaseSequence> BS_example = std::make_shared<OperatorWithSequence>(SMINUS, BO_op_1, BS_k1);
    context3.learn("op1", "stored", BS_example);
    context3.stored_learn("op2", "stored", BS_example);
    context3.print_universe();

    // Test ContextList.active_recall():
    std::cout << "op1 |stored>: " << context3.active_recall("op1", "stored").to_string() << std::endl;
    std::cout << "op2 |stored>: " << context3.active_recall("op2", "stored").to_string() << std::endl;

    // Test SelfKet:
    std::shared_ptr<BaseSequence> BS_self_ket = std::make_shared<SelfKet>(1, 5.2);
    std::cout << "SelfKet: " << BS_self_ket->to_string() << std::endl;
    context3.learn("op3", "Y", BS_self_ket);
    context3.stored_learn("op4", "Y", BS_self_ket);
    context3.print_universe();

    // Further SelfKet tests:
    std::shared_ptr<BaseSequence> BS_ket_3_op = std::make_shared<Ket>("op:", 3);
    std::shared_ptr<BaseSequence> BS_naked_self_ket = std::make_shared<SelfKet>(1);
    // std::shared_ptr<BaseOperator> BO_identity = std::make_shared<NumericOperator>(1); // Maybe implement EmptyOperator?
    std::shared_ptr<BaseOperator> BO_identity = std::make_shared<EmptyOperator>();
    OperatorWithSequence OpWithSeq_SelfKet(SPLUS, BO_identity, BS_ket_3_op);
    OpWithSeq_SelfKet.append(SMERGE2, BO_identity, BS_naked_self_ket);
    std::cout << "OpWithSeq_SelfKet: " << OpWithSeq_SelfKet.to_string() << std::endl;

    std::shared_ptr<BaseSequence> BS_op_self = std::make_shared<OperatorWithSequence>(OpWithSeq_SelfKet);
    context3.stored_learn("op", "*", BS_op_self);
    context3.memoize_learn("mem-op", "Z", BS_op_self);
    std::shared_ptr<BaseOperator> BS_clean_op = std::make_shared<SimpleOperator>("op");
    std::shared_ptr<BaseSequence> BS_clean_op_self = std::make_shared<OperatorWithSequence>(SPLUS, BS_clean_op, BS_naked_self_ket);
    // context3.stored_learn("op2", "Z", BS_clean_op_self);
    context3.learn("op2", "Z", BS_clean_op_self);
    context3.print_universe();
    std::cout << "op4 |Y>: " << context3.active_recall("op4", "Y").to_string() << std::endl;
    std::cout << "op |X>: " << context3.active_recall("op", "X").to_string() << std::endl;
    std::cout << "op2 |Z>: " << context3.active_recall("op2", "Z").to_string() << std::endl;
    std::cout << "mem-op |Z>: " << context3.active_recall("mem-op", "Z").to_string() << std::endl;


    // Hunt down empty sequence bug! Maybe it is not an empty sequence bug??
    //Sequence empty_seq;
    //std::shared_ptr<BaseSequence> bSeq_tmp = std::make_shared<Ket>();
    // std::shared_ptr<BaseSequence> bSeq_tmp = std::make_shared<Ket>("soup");
    // std::shared_ptr<BaseSequence> bSeq_tmp = std::make_shared<SelfKet>();
    // std::shared_ptr<BaseSequence> bSeq_tmp = std::make_shared<Superposition>();
    //std::shared_ptr<BaseSequence> bSeq_tmp = std::make_shared<Sequence>(empty_seq);
    // std::shared_ptr<BaseSequence> bSeq_tmp = std::make_shared<Sequence>("fish");
    // std::cout << "bSeq_tmp->size(): " << bSeq_tmp->size() << std::endl;
    //NewContext empty_seq_context("empty seq bug ... ");
    //empty_seq_context.learn(3, 4, bSeq_tmp);
    // empty_seq_context.recall("random", "random");
    //std::shared_ptr<BaseSequence> recall_result = empty_seq_context.recall(1000000, 2000000);
    //std::cout << "recall_result->size(): " << recall_result->size() << std::endl;
    // ContextList empty_seq_context_list("more empty seq bug ... ");
    // empty_seq_context_list.recall("random", "random");


    // Test Compound Constants:
    ConstantInteger c_int(37);
    ConstantFloat c_float(3.1415);
    ConstantString c_str(" text ");
    ConstantOperator c_op("op1");
    std::cout << "Constants: ";
    std::cout << c_int.to_string() << ", ";
    std::cout << c_float.to_string() << ", ";
    std::cout << c_str.to_string() << ", ";
    std::cout << c_op.to_string() << std::endl;

    // Test Compound Operator:
    /*
    CompoundOperator smap_op("smap");
    smap_op.append(&c_int);
    smap_op.append(&c_float);
    smap_op.append(&c_str);
    smap_op.append(&c_op);
    std::cout << "smap: " << smap_op.to_string() << std::endl;
    */

    // Test smart pointers:
    std::shared_ptr<CompoundConstant> c_int2 = std::make_shared<ConstantInteger>(42);
    std::shared_ptr<CompoundConstant> c_float2 = std::make_shared<ConstantFloat>(2.78);
    std::shared_ptr<CompoundConstant> c_str2 = std::make_shared<ConstantString>("constant string");
    std::shared_ptr<CompoundConstant> c_op2 = std::make_shared<ConstantOperator>("constant-op");
    seq3.dummy1(c_float2);
    std::vector<std::shared_ptr<CompoundConstant> > const_vec;
    const_vec.push_back(c_int2);
    const_vec.push_back(c_float2);
    const_vec.push_back(c_str2);
    const_vec.push_back(c_op2);
    seq3.dummy2(const_vec);

    // Test smart pointer CompoundOperator:
    CompoundOperator testing_op("test");
    testing_op.append(c_int2);
    testing_op.append(c_float2);
    testing_op.append(c_str2);
    testing_op.append(c_op2);
    std::cout << "CompoundOperator: " << testing_op.to_string() << std::endl;

    // Test OperatorLibrary:
    Ket some_ket("foo", 3);
    Ket some_ket2("Just a short sentence", 5);
    std::cout << "ssplit 3|foo>: " << ssplit(some_ket).to_string() << std::endl;
    std::cout << "ssplit[\" \"] 5|Just a short sentence>: " << ssplit(some_ket2, " ").to_string() << std::endl;
    std::shared_ptr<CompoundConstant> space_constant = std::make_shared<ConstantString>(" ");
    std::vector<std::shared_ptr<CompoundConstant> > const_vec2;
    const_vec2.push_back(space_constant);
    std::cout << "real: ssplit[\" \"] 5|Just a short sentence>: " << op_ssplit(some_ket2, const_vec2).to_string() << std::endl;

    // Test range():
    Sequence input_seq, start_seq("1"), stop_seq("10");
    std::cout << "srange(|1>, |10>): " << op_srange2(input_seq, start_seq, stop_seq).to_string() << std::endl;
    Sequence start_seq2("number: 3"), stop_seq2("number: 6"), stop_seq2b("price: 7");
    std::cout << "srange(|number: 3>, |number: 6>): " << op_srange2(input_seq, start_seq2, stop_seq2).to_string() << std::endl;
    std::cout << "srange(|number: 3>, |price: 7>): " << op_srange2(input_seq, start_seq2, stop_seq2b).to_string() << std::endl;
    Sequence start_seq3("4"), stop_seq3("7"), step_seq3("0.5");
    std::cout << "srange(|4>, |7>, |0.5>): " << op_srange3(input_seq, start_seq3, stop_seq3, step_seq3).to_string() << std::endl;
    Sequence step_seq4("-0.5");
    std::cout << "srange(|7>, |4>, |-0.5>): " << op_srange3(input_seq, stop_seq3, start_seq3, step_seq4).to_string() << std::endl;

    // Test float_to_int():
    long double test_float(123456789012);
    std::cout << "test_float: " << float_to_int(test_float, 4) << std::endl;
    test_float = 3.14159265;
    std::cout << "test_float: " << float_to_int(test_float, 5) << std::endl;
    std::cout << "test_float: " << float_to_int(test_float, 2) << std::endl;

    // Test arithmetic():
    Sequence symbol_seq("+");
    std::cout << "arithmetic(|1>, |+>, |10>): " << op_arithmetic3(input_seq, start_seq, symbol_seq, stop_seq).to_string() << std::endl;

    // Test extract functions:
    Ket category_ket("a: b: c: d: e");
    std::cout << "extract-head |a: b: c: d: e>: " << extract_head(category_ket).to_string() << std::endl;
    std::cout << "extract-tail |a: b: c: d: e>: " << extract_tail(category_ket).to_string() << std::endl;
    std::cout << "extract-category |a: b: c: d: e>: " << extract_category(category_ket).to_string() << std::endl;
    std::cout << "extract-value |a: b: c: d: e>: " << extract_value(category_ket).to_string() << std::endl;

    // Test simm:
    Superposition sp_1("a"), sp_2("b");
    sp_1.add("b");
    sp_1.add("c");
    std::cout << "simm(|a> + |b> + |c>, |b>): " << simm(sp_1, sp_2) << std::endl;

    // Test push-float and pop-float:
    Ket pi_ket("pi", 3.14159), random_ket("random: random");
    std::cout << "pi_ket: " << pi_ket.to_string() << std::endl;
    std::cout << "push-float pi_ket: " << push_float(pi_ket).to_string() << std::endl;
    std::cout << "pop-float push-float pi_ket: " << pop_float(push_float(pi_ket)).to_string() << std::endl;
    std::cout << "pop-float |random: random>: " << pop_float(random_ket).to_string() << std::endl;

    // Test split():
    Ket split_ket1("happy"), split_ket2("Just some text");
    std::cout << "split |happy>: " << split(split_ket1).to_string() << std::endl;
    std::cout << "split[\" \"] |Just some text>: " << split(split_ket2, " ").to_string() << std::endl;

    // Test rank:
    std::cout << "rank (|a> + |b> + |c>): " << rank(sp_1).to_string() << std::endl;

    // Test sp2seq and seq2sp:
    std::cout << "sp2seq (|a> + |b> + |c>): " << sp2seq(sp_1).to_string() << std::endl;
    std::cout << "seq2sp sp2seq (|a> + |b> + |c>): " << seq2sp(sp2seq(sp_1)).to_string() << std::endl;

    // Test bar-chart[10]:
    std::cout << "bar-chart[10] rank (|a> + |b> + |c>):\n" << bar_chart(rank(sp_1), 10).to_string() << std::endl;

    // Test normalize:
    std::cout << "normalize rank (|a> + |b> + |c>):\n" << rank(sp_1).normalize().to_string() << std::endl;

    // Test SimpleOperator(""):
    Sequence simple_op_test("fish", 5);
    SimpleOperator simple_op("");
    std::cout << "SimpleOperator(\"\"): " << simple_op.to_string() << std::endl;
    std::cout << "SimpleOperator(\"\").Compile(): " << simple_op.Compile(context3, simple_op_test).to_string() << std::endl;

    // Test split[""] and ssplit[""] and Timer:
    {
        Timer_us timer("timing split");
        Ket split_ket("happy");
        std::cout << "split[\"\"] |happy>: " << split(split_ket, "").to_string() << std::endl;
        std::cout << "ssplit[\"\"] |happy>: " << ssplit(split_ket, "").to_string() << std::endl;
    }

    // Test display_time():
    long long testing_time = 1000 * 60 * 60 * 5 + 1000 * 60 * 37 + 1000 * 19 + 1;
    std::cout << "testing_time: " << display_time(testing_time) << std::endl;

    // Test FunctionOperatorMap:
    fn_map.print();

    // Test CompoundOperator, split and ssplit:
    Sequence ket_list("apple, orange, banana, mandarin, pear");
    // CompoundOperator split_op("split"), ssplit_op("ssplit");
    std::shared_ptr<CompoundConstant> comma_constant = std::make_shared<ConstantString>(", ");
    // std::vector<std::shared_ptr<CompoundConstant> > const_vec3;  // add constructors, so we don't have to manually create const_vec's.
    // const_vec3.push_back(comma_constant);
    // CompoundOperator split_op("split", const_vec3), ssplit_op("ssplit", const_vec3);
    CompoundOperator split_op("split", comma_constant), ssplit_op("ssplit", comma_constant);
    std::cout << "split: " << split_op.to_string() << std::endl;
    std::cout << "ssplit: " << ssplit_op.to_string() << std::endl;
    std::cout << "split.Compile: " << split_op.Compile(context3, ket_list).to_string() << std::endl;
    std::cout << "ssplit.Compile: " << ssplit_op.Compile(context3, ket_list).to_string() << std::endl;

    std::shared_ptr<CompoundConstant> width_constant = std::make_shared<ConstantInteger>(15);
    CompoundOperator barchart_op("bar-chart", width_constant);
    std::cout << "bar-chart: " << barchart_op.to_string() << std::endl;
    Superposition some_sp("a", 3);
    some_sp.add("b", 5);
    some_sp.add("c", 7);
    some_sp.add("d", 9);
    Sequence some_seq(some_sp);
    std::cout << "bar-chart.Compile:\n" << barchart_op.Compile(context3, some_seq).to_string() << std::endl;

    std::shared_ptr<CompoundConstant> start_constant = std::make_shared<ConstantInteger>(4);
    std::shared_ptr<CompoundConstant> stop_constant = std::make_shared<ConstantInteger>(8);
    CompoundOperator range_sigmoid("sigmoid-in-range", start_constant, stop_constant);
    std::cout << "sigmoid: " << range_sigmoid.to_string() << std::endl;
    std::cout << "sigmoid.Compile: " << range_sigmoid.Compile(context3, some_seq).to_string() << std::endl;

    // Test Sequence::multiply:
    Sequence seq_mult = sp2seq(some_sp);
    seq_mult.multiply(2);
    std::cout << "seq_mult: " << seq_mult.to_string() << std::endl;

    // Test SimpleOperator seq2sp:
    SimpleOperator the_seq2sp_op("seq2sp");
    std::cout << "seq2sp seq_mult: " << the_seq2sp_op.Compile(context3, seq_mult).to_string() << std::endl;

    // Test coeff_sort() and ket_sort():
    Superposition unsorted_sp("a", 3.7);
    unsorted_sp.add("b", 0.2);
    unsorted_sp.add("c", 5);
    unsorted_sp.add("d", 4);
    std::cout << "unsorted_sp: " << unsorted_sp.to_string() << std::endl;
    unsorted_sp.coeff_sort();
    std::cout << "coeff-sort unsorted_sp: " << unsorted_sp.to_string() << std::endl;
    unsorted_sp.ket_sort();
    std::cout << "ket-sort unsorted_sp: " << unsorted_sp.to_string() << std::endl;
    unsorted_sp.coeff_sort();
    unsorted_sp.natural_sort();
    std::cout << "natural-sort unsorted_sp: " << unsorted_sp.to_string() << std::endl;
    SimpleOperator coeff_sort_op("coeff-sort");
    std::cout << "coeff_sort_op.Compile: " << coeff_sort_op.Compile(context3, unsorted_sp).to_string() << std::endl;

    // Test FunctionOperator:
    Ket param0("x", 3), param1("y"), param2("z", 5.2);
    FunctionOperator fn_op1("foo1", std::make_shared<Ket>(param0));
    FunctionOperator fn_op2("foo2", std::make_shared<Ket>(param0), std::make_shared<Ket>(param1));
    FunctionOperator fn_op3("foo3", std::make_shared<Ket>(param0), std::make_shared<Ket>(param1), std::make_shared<Ket>(param2));
    std::cout << "foo1: " << fn_op1.to_string() << std::endl;
    std::cout << "foo2: " << fn_op2.to_string() << std::endl;
    std::cout << "foo3: " << fn_op3.to_string() << std::endl;
    Ket param3("3"), param4("7");
    Sequence empty_seq("");
    FunctionOperator fn_op4("srange", std::make_shared<Ket>(param3), std::make_shared<Ket>(param4));
    std::cout << "srange(|3>, |7>): " << fn_op4.to_string() << std::endl;
    std::cout << "srange(|3>, |7>).Compile: " << fn_op4.Compile(context3, empty_seq).to_string() << std::endl;

    // Test Superposition::select:
    Superposition select_test_sp("a");
    select_test_sp.add("b");
    select_test_sp.add("c");
    select_test_sp.add("d");
    select_test_sp.add("e");
    select_test_sp.add("f");
    std::cout << "select_test_sp: " << select_test_sp.to_string() << std::endl;
    std::cout << "select_test_sp.select[1,-1]: " << select_test_sp.select(1,-1).to_string() << std::endl;
    std::cout << "select_test_sp.select[2,-2]: " << select_test_sp.select(2,-2).to_string() << std::endl;

    // Test rel-kets[op]:
    // std::vector<ulong> vec = context3.relevant_kets("*");
    std::vector<ulong> vec = context3.relevant_kets("op2");
    Superposition rel_kets_sp(vec);
    context3.print_universe();
    std::cout << "rel_kets_sp: " << rel_kets_sp.to_string() << std::endl;
    // CompoundOperator rel_kets_op("rel-kets", std::make_shared<ConstantOperator>("*"));
    // CompoundOperator rel_kets_op("rel-kets", std::make_shared<ConstantOperator>("op2"));
    CompoundOperator rel_kets_op("rel-kets", std::make_shared<ConstantOperator>("*"), std::make_shared<ConstantOperator>("op2"));
    std::cout << "rel_kets_op: " << rel_kets_op.to_string() << std::endl;
    std::cout << "rel_kets_op.Compile: " << rel_kets_op.Compile(context3, some_seq).to_string() << std::endl;

    return 0;
}
