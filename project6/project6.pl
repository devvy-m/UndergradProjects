% Devon Layton, CSCI 3210
% Project6, Due: 04/14/20

% This program will demonstrate the use of the logical programming language Prolog
% Only built-in functions covered in class may be used, given predicate formats must be used,
% All solutions provided in one sources file with comments for each user-defined predicate


% The predicate mymember(value,list) defines rules to see if a value is a member of a list
%if first element is in list, then it is a list

%mymember(value, list)
mymember([]).
mymember(X, [X|_]).
    % mymember(X, [_|List]):- mymember(X, List).

% Design rules to check if an integer is an even number or not. 
% if a number modded by 2 is 0, then it is even

% myeven(a_number)
myeven(N) :-
    0 is N mod 2.

%Design rules to find out the number of even values in a list of integers. 
% Is a recursive call that checks for empty list, checks the head element if it's even
% and icrements if it is, and then checks through the rest of the list
%myevennumber(a_number, list)
myevennumber(0, []).
myevennumber(N, [H|T]) :-
    myevennumber(M, T),
    myeven(H),
    N is M+1.
%odd number check
myevennumber(N, [_|T]) :-
    myevennumber(N, T).


%Design rules to find the minimum value in a list of integers.
%checks for empty list(false) or 1 element list
%then iterates through the list checking if the head is minium in the head element
% or the tail list
%myminlist(list, minimum_value)
myminlist([]).
myminlist([_]).
myminlist([X], X).
% myminlist([X], X).
myminlist([H|T], H) :-
    myminlist(T, Y),
    H<Y.
myminlist([H|T], X) :-
    myminlist(T, X),
    H>X.

%Design rules to find out whether a list is a palindrome
%first check is empty list, next is one with only one element,
%Palindrome list uses recursion to check the tail of the list with
%the head of the original list rather than having to use a reverse

%palindrome(list)
palindrome([]).
palindrome([_]).
palindrome(List) :-
    append([H|T], [H], List),
    palindrome(T).
    
