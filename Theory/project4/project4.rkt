#lang racket

; Devon Layton, CSCI 3210
; Project 4, Due: 03/02/2020
; This program will demonstrate the use of the functional programming language Scheme
; Only functional features of the language will be used

; Non-recursive Scheme function area(r) that given the radius r of a circle,
; computes its area as 3.14 * r * r. I defined pi as a constant to be used in area function

(define pi 3.14)

(define(area r)
  (* pi r r))

; Write a recursive Scheme function power(A, B) that takes two integer parameters,
; A and B, and returns A raised to the B power. A must be a positive value, but B maybe a negative value.
; number raised to itself is 1
(define(power A B)
  (cond
    ((= B 0) 1)
    ((> B 0) (* A (power A (- B 1))))))

; Recursive Scheme function countZero(list) that returns the number of value zeros
; in a given simple list of numbers. Car list checks the first element in list
; if equal to 0 and increments if it is. Otherwise, it recurses throught list elements

(define(countZero list)
  (cond
    ((null? list)
     0)
    ((= (car list) 0)
     (+ 1 (countZero (cdr list))))
    (else(countZero (cdr list)))))

; Recursive Scheme function reverse(list) that returns the reverse of its simple list parameter. 
; Base case is an empty list, each recursive call stores the last element in the initial
; list. Upon exiting recursive calls, the element stored in that call is appended to new list's body

(define(reverse List)
   (if (null? List)
       '()
       (append (reverse (cdr List))(list (car List)))))

; Recursive Scheme function palindrome(list) that returns true if the simple list
; reads the same forward and backward; otherwise returns false.
; if the list is empty, an empty list is equal to itself, otherwise compare
; given list to itself reversed. I utilize the function reverse instead of re-defining it

(define(palindrome List)
  (cond
    ((null? List)
     true)
    ((equal? List (reverse List))
     true)
    (else false))
 )

; Recursive Scheme function merge(firstNameList, lastNamelist) that receives a simple list
; of first names and a simple list of last names, and merges the two lists into one list
; consisting of (firstName lastName) pairs. If either list is null, then it is an empty list
; other wise, pair(using racket's cons) the first elements of both lists and recursively
; call merge for the rest of the list's body

(define(merge firstNameList lastNameList)
  (cond
    ((null? firstNameList)
     '())
    ((null? lastNameList)
     '())
    (else(cons (list (car firstNameList) (car lastNameList))(merge (cdr firstNameList) (cdr lastNameList))))))
    
     
