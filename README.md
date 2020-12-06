# Semantic DB 3.1

This project is a C++ rewrite of the original python [Semantic DB](https://github.com/GarryMorrison/Semantic-DB). It largely works now, but we are still in the debug stage, and as such, it still contains bugs. But hopefully, not too many of them.

What is the Semantic DB
-----------------------

The SDB is a mathematical notation to represent states of a system, and changes in state of that system. We represent states using `kets` and changes in state with `operators`. That's it. Everything else is built on top of this premise. This notation can be used to represent knowledge in say the semantic web, graph databases, and more speculatively, neural circuits. Indeed, the notation is intended to be general, and represent almost anything of interest.

What are Kets
-------------

The essence of a ket is a string, float pair, with a slightly unusual notation borrowed from [quantum mechanics](https://en.wikipedia.org/wiki/Bra%E2%80%93ket_notation). Mathematically, the string can be considered the label for a basis element in a vector space, and the float is the corresponding coefficient. With this in mind, it makes sense that we can add kets together in a natural way, and we call such an object a superposition. Depending on the context, superpositions are a sparse vector in an almost infinte, real valued, abstract vector space (the size of the space is only limited by the finite length of strings), but they can also be lists of objects, or a collection of verticies in a graph, or the currently active nodes in a network, or the currently active synapses in a neural circuit, or a mathematical set of elements, or a fuzzy set of elements, or a collection of objects and their corresponding probabilities, and so on. Because the notation is general, the actual interpretation of a superposition depends on the context. We can also consider changes in superpositions over time, one method to do so is using operators, the other is to consider sequences of superpositions. Sequences are an important addition to the notation because otherwise certain things would be impossible, or at least difficult. It should be noted that sequences of length one are simply superpositions, so a sequence is a more general object than a superposition, which in turn is a more general object than a ket. So if below we refer to an object being a sequence, it may also just be a superposition, or for that matter a single ket. Note that sequences are time ordered lists of superpositions, and superpositions are positions in some vector space, so it is not immediately clear what kind of mathematical object a sequence is. I mean, what is the interpretation of a time ordered list of vectors? But we can work with them nonetheless.


Some Kets
---------

Let us give some concrete examples of kets. The person Fred Smith could be represented as `|person: Fred Smith>` where the `type` or `category` is given explicitly, in this case `person`. But more commonly we leave out the type: `|Fred Smith>`. An apple could be written as `|food: apple>` again, with explicit type, or without the type: `|apple>`. Seven apples can be written as `7|apple>`. Note that if the coefficient is not given, then it is by default 1. If a ket is not in the given superposition, we can add it with coefficient 0, without changing the `meaning` of that superposition (for some defintion of meaning). If a ket has the empty label `|>` we call it the empty ket, and is treated as the identity element for superpositions. Ie, we can add it to any superposition, and it will not change the superposition. Kets with negative coefficients are much less common, but usually represent inhibition of some sort (ie, the activation of that node is strongly suppressed). We can of course also represent fuzzier quantities by using non integer coefficients. Slightly hungry might be represented as `0.3|hungry>`, and very hungry as `0.9|hungry>`. We can also represent probabilities using non integer coefficients, providing the sum of the coefficients in the superposition sum to 1 (this can be arranged using the `normalize` operator).


What are Operators
------------------

In the most general sense, an operator is an object that takes a sequence and returns a new sequence. Given the input type to an operator is the same as the output type, we can easily chain operators into so called operator sequences, where we apply a series of operators to the input sequence. If we consider a sequence as the current position in some abstract space, then an operator acts to change that position. And an operator sequence acts to step through that abstract space. So in some strong sense an operator sequence is a pathway through that space (indeed, if we represent positions on a map with kets, and represent steps between those kets with operators, the operator sequence would be a literal pathway). While kets all look the same once you get used to them, operators in contrast come in a great variety. Indeed, a central focus of the semantic db project is working out what operators are useful or required to complete some task. Thankfully the code is modular, so adding new operators to the operator library is relatively easy. 


Linear Operators
----------------

The simplest operators are linear, and can be considered a type of sparse matrix multiplication. They also work with sequences, and preserve the sequence structure (ie, a kind of generalization of the definition of linear). So for example if some operator `op` is linear then we have the following property:
```
op (c1|x> + c2|y> + c3|z>) == c1 op |x> + c2 op |y> + c2 op |z>
```
where `{c1, c2, c3}` are float coefficients, and `|x>, |y>, |z>` are basis elements. Further, if we have several linear operators, `op1, op2, op3, op4` then it follows that an operator sequence composed from them, is also linear:
```
op4 op3 op2 op1 (c1|x> + c2|y>) == c1 op4 op3 op2 op1 |x> + c2 op4 op3 op2 op1 |y>
```
And in fact this operator sequence also represents sparse matrix multiplication (a matrix times a matrix is also a matrix). Linear operators also preserve sequence structure, so for example:
```
op (c1|a> + c2|b> . c3|x> + c4|y> + c5|z>) == c1 op|a> + c2 op|b> . c3 op|x> + c4 op|y> + c5 op|z>
```
where `.` is used to separate the superpositions in a given sequence. Here our sequence is length 2.


Sigmoids
--------

We have a further type of operator called `sigmoids`, which are almost linear, in the sense they preserve sequence structure, but they do change the coefficients of kets. The most common sigmoid is the `clean` sigmoid that sets the coefficient to 0 if it is less than or equal 0, else 1. But we also have an [ReLU](https://en.wikipedia.org/wiki/Rectifier_(neural_networks)) sigmoid, that maps any negative coefficient to 0, else leaves it unchanged. The point is, deep learning models should have a compact representation in the semantic db, but unfortunately likely too slow to be practical in most cases. If the required matrices are very sparse however, then the semantic db might be faster.


Non Linear Operators
--------------------

Most of the interesting operators in the semantic db are actually non-linear. And they are all quite varied in what they do, so it is hard to summarize them. But we do have distinct classes of operators, which is very obvious in the back-end code. We have `simple` operators that take no parameters such as `drop`, `normalize`, `pick`, etc. We have `numeric` operators that multiply all the kets in a sequence by a value, such as `7`. We have `compound` operators that take constant parameters, such as `table[label, op1, op2]` or `plus[3]`. And finally operators that look like functions, such as `srange(|1>, |10>)` or `arithmetic(|3>, |+>, |5>)`. And they are further broken down into sub-classes of these, depending on if they require `context`, or if they respect sequence strucuture or not, and so on. We have a working convention that the sequence version of an operator starts with s. So for example `how-many` counts the number of kets in a superposition, meanwhile `show-many` counts the number of superpositions in a sequence. Or `split` splits a ket into a superposition, and `ssplit` splits a ket into a sequence, and so on.


ContextList
-----------

So above we mentioned representing knowledge, but haven't yet presented how to do so. This is where `context` and `learn rules` come in (see next section). A `context` is simply a collection of knowledge that is distinct, and fully independent from, other collections of knowledge that we may have. Think of them a little like a namespace in a programming language, or a scope. So for example your knowledge about your friends might be better represented as distinct from your knowledge about your work colleagues. This might look a little like:
```
|context> => |friends>
... some knowledge about friends ...

|context> => |work colleagues>
... some knowledge about work ...
```
A context list is a collection of different contexts. To see the contextlist in the semantic db shell, enter: `dump multi`.


Learn Rules
-----------

Finally, we are ready to describe `learn rules`. A learn rule is what we use to associate a sequence with a given ket, labelled with an operator. After we have done so, that operator applied to that ket will return the learnt sequence. Recalling that when we say `sequence` it can be a proper sequence, or a superposition, or just a ket (the latter are just special types of sequences). Say we want to learn a little knowledge about Fred:
```
full-name |Fred> => |Fred Smith>
mother |Fred> => |June>
father |Fred> => |Robert>
age |Fred> => |34>
friends |Fred> => |Jack> + |Sam> + |Emma> + |Liz>
spelling |Fred> => |F> . |r> . |e> . |d>
```
where the general structure for a learn rule is:
```
operator ket learn-rule-type sequence
```
Note that the right hand side of this quad is a sequence, not just a ket. This makes the SDB significiantly more powerful in what it can represent than a standard triple store, such as those based on RDF.

So now we have entered that knowledge into the SDB shell, the operators `full-name`, `mother`, `father`, `age`, `friends` and `spelling` are all defined when applied to `|Fred>`. But since we know nothing about `|Tim>`, those operators have no meaning when applied to his ket. Frequently when the system is asked a question it doesn't know the answer, the system returns the empty ket `|>`, otherwise known as the `don't know` ket. We even have a special operator `do-you-know` that returns `|no>` when applied to `|>` and `|yes>` otherwise. This is often useful in if statements.

It should be noted that the Semantic DB is an `open model` in the sense that we do not have a fixed `schema`. So we are not restricted to a small list of operators, or that all kets must have the same set of operators defined. For example, perhaps we don't know the `mother`, or `father`, or `age` of `|Tim>`, so we leave them undefined. But maybe we do know `favourite-wine` and `favourite-song` for `|Tim>` but not for `|Fred>`.


Other types of Learn Rules
--------------------------

It must be noted at this point that we have more than one type of learn rule. The complete set of types are `=>`, `+=>`, `.=>`, `#=>` and `!=>`. Respectively they are called `learn-rule`, `add-learn-rule`, `seq-learn-rule`, `stored-learn-rule` and `memoizing-learn-rule`. 

**Learn-rule**: sets the right hand sequence to the given value, over-writing any previous definition.

**Add-learn-rule**: adds the right hand sequence to any previous value. So for example:
```
friends |Tim> +=> |Fred>
friends |Tim> +=> |Beth>
friends |Tim> +=> |Mary>
```
is effectively the same as all at once:
```
friends |Tim> => |Fred> + |Beth> + |Mary>
```

**Seq-learn-rule**: is similar, but sequence appends instead of adding. So for example:
```
friends |Tim> .=> |Fred>
friends |Tim> .=> |Beth>
friends |Tim> .=> |Mary>
```
is effectively the same as all at once:
```
friends |Tim> => |Fred> . |Beth> . |Mary>
```
Though I'm not sure why friends would be time ordered. Maybe we need a better example.

**Stored-learn-rule**:
Stores the right hand side in memory (in a compact form, after it has been parsed), and only evaluates it on invoke time. Most of the interesting user defined operators are of this type.

**Memoizing-learn-rule**:
Very similar to stored-learn-rule but with one big difference. Once the right hand side is evaluated, it is then written in place, or [memoized](https://en.wikipedia.org/wiki/Memoization). This is used when you don't want to calculate a value over and over again. A good use case is the Fibonacci sequence, or factorial:
```
Fib |0> => |0>
Fib |1> => |1>
Fib |*> !=> arithmetic(Fib minus[1] |_self>, |+>, Fib minus[2] |_self>) |>
```
where after we invoke `Fib |10>` we now know:
```
Fib |0> => |0>
Fib |1> => |1>
Fib |*> !=>  arithmetic( Fib minus[1]|_self>, |+>, Fib minus[2]|_self> ) |>
Fib |2> => |1>
Fib |3> => |2>
Fib |4> => |3>
Fib |5> => |5>
Fib |6> => |8>
Fib |7> => |13>
Fib |8> => |21>
Fib |9> => |34>
Fib |10> => |55>
```
If we had used a stored rule instead for the `Fib |*>` learn rule, none of the results would have been permanently added to our knowledge.


Label Descent
-------------

We can also learn operators with respect to whole classes of kets, by using the `|*>` ket, and `self-kets` written as `|_self>`. Eg, if a learn rule (it doesn't matter the type) is defined with respect to `|*>` then that operator is valid for any ket. This is considered a general rule, and can be over-written by specific rules. Consider for example plurals. The general rule for a plural is:
```
plural |*> #=>  |_self> _ |s>
```
where `|_self>` is replaced by the ket `plural` is applied to at invoke time, and `_ |s>` means append an "s" to this. But plurals have many exceptions, and for those we define specific rules that have higher precedence than the general rules. So for example:
```
plural |mouse> => |mice>
plural |tooth> => |teeth>
plural |foot> => |feet>
plural |radius> => |radii>
plural |matrix> => |matrices>
plural |elf> => |elves>
...
```
So now if we ask the system what is the plural of `|foot>` it will return `|feet>`, but if we ask the plural of `|apple>` it invokes the general rule and returns `|apples>`. Clearly, the human brain is doing something very similar to this. We can use the same idea to learn inverse plurals:
```
inverse-plural |*> #=>  remove-suffix["s"]|_self>
inverse-plural |mice> => |mouse>
inverse-plural |teeth> => |tooth>
inverse-plural |feet> => |foot>
inverse-plural |radii> => |radius>
inverse-plural |matrices> => |matrix>
inverse-plural |elves> => |elf>
...
```

Likewise, we can define general rules that apply to only specific classes of kets, instead of all kets. Say we want `op` to be defined only with respect to kets of type `food`:
```
op |food: *> #=> ... some expression ...
```
Again, specific rules always have higher priority than general rules. And is one of the use cases where it actually makes sense to define the type for your kets.


Ket Representation for Operators
--------------------------------

It is often useful to represent operators as kets. In this case the convention is to use the `type`/`category` `op:` and then the operator name. So for example, the `age` operator has ket representation `|op: age>`, and the `spelling` operator has ket representation `|op: spelling>`. Indeed, for every ket defined in the system, we have a special operator called `supported-ops` that keeps a superposition/list of all operators defined for that ket. So given the above knowledge about Fred, `supported-ops |Fred>` returns this superposition:
```
|op: full-name> + |op: mother> + |op: father> + |op: age> + |op: friends> + |op: spelling>
```
Further, it is used much less frequently, but we also have a ket representation for operator sequences. In this case we use `ops:` as the category. Eg, the `age` of someones `mother` would be `|ops: age mother>`. Note that only so called `simple` operators have a ket representation, so for example compound and function operators are excluded.

Filter()
--------

When working with knowledge it is often useful to filter a given sequence to those kets that satsify certain properties. For this, we have the filter function:
```
filter(operators, conditions) input-sequence
```
Often combined with the `rel-kets[op]` operator, which returns a superposition of all kets that have `op` defined for them. And `rel-kets[*]` that returns a superposition of all kets that have any operator defined for them. Note that if a ket is only specified on the right hand of a learn rule, but never on the left, rel-kets will not return it. If this is a problem, that could be fixed using the `find-inverse[op]` operator, which learns all the inverse links (the `inverse-plural` operator mentioned above is an example of an inverse link). So, if we want to filter all known kets (in the current context) to those that are furniture we would use the following:
```
filter(|op: is-furniture>, |yes>) rel-kets[*]
```
Or if we want to find all people that have father's with occupation doctor:
```
filter(|ops: occupation father>, |doctor>) rel-kets[*]
```
Functions are just a special type of operator, so of course they too can be chained in an operator sequence. Say we wish to find all humans, that are American, and are politicians, we would use:
```
filter(|op: is-human>, |yes>) filter(|op: is-american>, |yes>) filter(|op: occupation>, |politician>) rel-kets[*]
```
With the note that it will be faster to use the most strict condition first, in this case `occupation == politician`.


A Similarity Measure
--------------------

Though our sequence space, for lack of a better term, does not have a natural inner product (that is why we don't use the bra component of the bra-ket notation), we do have a similarity measure called simm(). This has the useful property of returning 1 if the given sequences are identical, 0 if they share no kets in common, and values in between otherwise. Note that all kets with coefficients <= 0 have no influence on the result. Mathematically I guess we could say this by saying, for any superpositions `sp1`, `sp2` and `X`:
```
simm(sp1 + 0 sp2, X) == simm(sp1, X)
```
where the 0 is taken as multiplying all the kets in `sp2`, and setting them to 0. Given the ease of which we can add operators to the operator library, if needed we could implement a dot product function. But so far it has not proved necessary. If you find an encoder that encodes an object to a superposition, or sequence, that carries the semantic meaning of that object, then simm() can be used to compare the semantic similarity of those objects. In general it is difficult to find such encoders.


Similar Input
-------------

Similar-input\[op\] is a very powerful operator that makes use of our similarity measure. A common usage is an operator such as:
```
pattern-simm (*) #=> drop-below[0.8] similar-input[pattern] |__self>
```
Given an input sequence, `similar-input` looks for all kets that have `pattern` defined, then compares those sequences with the input sequence. The output of `similar-input` is a superposition of those kets, with the coefficient being the similarity score. `drop-below[0.8]` then filters that superposition to kets with similarity score of at least 80%. Other values can of course be chosen, say 0.98 for 98% similarity, or 0.5 for 50% similarity, depending on how strict you want or need the similarity to be.


If-Then Machines
----------------

Next, we will describe if-then machines, a simplified model of a neuron. The general idea is for each `neuron` we have a collection of patterns defined, where those patterns are sequences. When we invoke our if-then machines, if the input sequence matches one of those patterns with sufficiently high similarity (specified using drop-below), then the `neuron` fires, and the output pattern is returned. In practice it seems the best results are when the output pattern is a ket or a superposition, rather than a sequence of length greater than 1. An abstract example of a single if-then machine looks like:
```
pattern |node: 1: 1> => seq1
pattern |node: 1: 2> => seq2
...
pattern |node: 1: n> => seqn
then |node: 1: *> => output-sp
```
where `seq1` ... `seqn` are all the input patterns that can potentially trigger this neuron. `output-sp` is the output superposition if it does trigger. And the kets `|node: 1: k>` for k in `{1,2,...,n,*}` can be considered the cell body of the neuron. Of course, in practice you have multiple if-then machines, using different ket labels for their cell bodies. Here we have the outline for m distinct if-then machines, for simplicity they all have n patterns, but that is not a requirement:
```
pattern |node: 1: 1> => seq1_1
pattern |node: 1: 2> => seq1_2
...
pattern |node: 1: n> => seq1_n
then |node: 1: *> => output-sp1

pattern |node: 2: 1> => seq2_1
pattern |node: 2: 2> => seq2_2
...
pattern |node: 2: n> => seq2_n
then |node: 2: *> => output-sp2

...

pattern |node: m: 1> => seqm_1
pattern |node: m: 2> => seqm_2
...
pattern |node: m: n> => seqm_n
then |node: m: *> => output-spm
```
The if-then machines are then invoked with an operator such as:
```
invoke-if-then (*) #=> then drop-below[0.9] similar-input[pattern] |__self>
``` 
It should be noted that if any of the output superpositions contain kets with negative coefficients, they will suppress the output of those kets from other activated neurons. Hence why negative coefficient kets are considered a type of inhibition. Note also, that if you want systems of if-then machines that do not interact, you need to define them with distinct operators. Say, instead of `pattern` use `layer1:pattern`, `layer2:pattern`, ..., `layern:pattern` and so on.


How To Define If-Then Machines
---------------------------

So we have an abstract representation of a neuron. But now what? Real neurons have the rather nice property that they configure themselves. Our if-then machines are distinctly hand written. In general I don't have an answer for this. Certainly we can, and have, set up .sw3 files that self build if-then machines that act as a spell checker, or detect ngrams of integer sequences, or extract knowledge from short sentences. But it is not even known how to apply if-then machines to elementary visual tasks such as MNIST digit recognition. That would of course be solved if we could find a good encoder for MNIST digits. And how do you go about defining very large numbers of if-then machines, given the human brain has vast numbers of neurons!


Semantic DB Shell
-----------------

Once the main project is compiled (hopefully it is not too difficult to do so), we interact with the system by way of the SDB shell. This is where we load, type, interact and then save knoweldge. Invoke on the command line with `--help` to see brief usage information. Invoke with `-i` or `--interactive` to enter the shell. Once in the shell, type `help` to see the inline help message. Type `usage` to see a list of operators available in our operator library. Type `usage op` to see the specific usage information for the specified operator (note that currently some operators do not have usage information defined, but that will slowly improve).

