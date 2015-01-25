%% TBox
 a :- b,    not p1.
   :- a, b, not p2.
 d :- c,    not p3.
   :- d, c, not p4.

%% ABox
b.
c.

%% Choice rules
p1 :- not a1.
a1 :- not p1.
p2 :- not a2.
a2 :- not p2.
p3 :- not a3.
a3 :- not p3.
p4 :- not a4.
a4 :- not p4.


%#minimize[p3,p4,p1,p2].