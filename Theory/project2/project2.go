//Author: Devon Layton

// -- Go does not have any classes but you can define methods of types
package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
)

//All lower case names to indicate private member data
//Student struct function definition to calculate the final letter grade
type grade func(int, int, int, int, int) int

// Go does not have explicit classes, methods and members are defined in structs instead
// lettergrade will be calculated with finalLetterGrade function dependent on the values finalNumberGrade function returns
type student struct {
	id, letterGrade                                   string //id read from file, letterGrade value assigned from finalLetterGrade function
	claScores, olaScores, quizScores, exam, finalExam int    //read from file
	totalPoints                                       int    //total points from all scores calculated by finalNumeberGrade member function
	finalNumberGrade                                  grade  //function as a field
	finalLetterGrade                                  func() //function  finalLetterGrade to find the Letter grade based on the number grade from Grade function
}

// Roster - map of students
type roster struct {
	count    int                //tracks the number of students in the file
	studInfo []student          //empty slice of struct pointers used for statistic information
	smap     map[string]student //keys in the map will either be a student id or lettergrade
	query    func()             //query function maitains all query and print information for students
}

// Member of student to calculate the letter grade value based on total points
func finalLetterGrade(in int) string {
	var letter string //letter variable is assigned a value based on the switch cases & function returns value to struct field letterGrade
	switch {
	case in >= 90:
		letter = "A"
	case in >= 87 && in < 90:
		letter = "B+"
	case in >= 83 && in < 87:
		letter = "B"
	case in >= 80 && in < 83:
		letter = "B-"
	case in >= 77 && in < 80:
		letter = "C+"
	case in >= 73 && in < 77:
		letter = "C"
	case in >= 70 && in < 73:
		letter = "C-"
	case in >= 67 && in < 70:
		letter = "D+"
	case in >= 63 && in < 67:
		letter = "D"
	case in >= 60 && in < 63:
		letter = "D-"
	default:
		letter = "F"
	}
	return letter
}

// Function to query information about a student, because roster is a map of student structs, we call the function with the
// roster map but pass that student key to the function to see if the student exists in the map, if not throw the error
func findStudent(s []student, find func(student) bool) {
	for _, v := range s {
		if find(v) == true {
			fmt.Println("\n", v.id, ":", v.claScores, v.olaScores, v.quizScores, v.exam, v.finalExam, v.totalPoints)
		}
	}
}

func query(rost *roster) {

	rost.studInfo = make([]student, 0, len(rost.smap)) //initialize a slice the size of the map to maintain a stable iteration order over the map pairs
	//slice information
	for _, stud := range rost.smap {
		rost.studInfo = append(rost.studInfo, stud)
	}

	//sort by cnumbers to display in  sorted order
	sort.SliceStable(rost.studInfo, func(i, j int) bool {
		return rost.studInfo[i].id < rost.studInfo[j].id
	})

	for i := 0; i < 2; i++ {
		fmt.Print("\nEnter C# beginning with a 'c': ") //prompt user for input
		var inCnumber string                           //file variable
		fmt.Scanln(&inCnumber)                         //scan command line for input
		if len(inCnumber) < 8 {
			print("\nInvalid C-number \n")
		} else {
			findStudent(rost.studInfo, func(s student) bool { //call function to query the student
				if s.id == inCnumber {
					return true
				}
				return false
			})
		}
	}
	fmt.Println("\n C - Number:\t Cla \t Ola \t Quiz \t Exam \t Final \t TotalPoints \t LetterGrade")
	fmt.Println("---------------------------------------------------------------------------------------------")
	//initialize variables to first index value to be checked for max value
	highCla := rost.studInfo[0].claScores
	highOla := rost.studInfo[0].olaScores
	highQuiz := rost.studInfo[0].quizScores
	highExam := rost.studInfo[0].exam
	highFinalExam := rost.studInfo[0].finalExam
	var sum int

	// for each interation of the loop, check if each value is greater than the last; if so, update the variable to the correct max value
	// to check the average, add each index's total points which will be divided by that roster's count(number of items)
	for _, k := range rost.studInfo {
		sum += k.totalPoints

		fmt.Println(" ", k.id, ":\t ", k.claScores, " \t ", k.olaScores, " \t ", k.quizScores, " \t ", k.exam, " \t ", k.finalExam, " \t ", k.totalPoints, "\t \t |", k.letterGrade)
		if k.claScores > highCla {
			highCla = k.claScores
		}
		if k.olaScores > highOla {
			highOla = k.olaScores
		}
		if k.quizScores > highQuiz {
			highQuiz = k.quizScores
		}
		if k.exam > highExam {
			highExam = k.exam
		}
		if k.finalExam > highFinalExam {
			highFinalExam = k.finalExam
		}
	}
	//the actual average is caluculated in the print statement, sum/ (# of elements in the map)
	fmt.Println("---------------------------------------------------------------------------------------------\n Average:", (sum / rost.count))
	fmt.Println(" Highest Cla \t Highest Ola \t Highest Quiz \t Highest Exam \t Highest Final Exam")

	//print out the highest scores
	fmt.Println("   ", highCla, "\t \t   ", highOla, "\t \t\t", highQuiz, "\t\t", highExam, "\t\t", highFinalExam)

}

func main() {

	studRoster := new(roster)        //instantiate new roster object to store all Student Objects
	fmt.Print("\nEnter file name: ") //prompt user for input
	var inFile string                //file variable

	fmt.Scanln(&inFile)              //scan command line for input
	fmt.Print("\n")                  //print a newline before next prompts
	dataFile, err := os.Open(inFile) //open the file and check for errors opening

	if err != nil { //if there is an error, print error and quit program (error is a built into golang file io)
		log.Fatalf("Could not open file: %s", err)
	}

	scanW := bufio.NewScanner(dataFile) //create a buffer for input
	scanW.Split(bufio.ScanLines)        //split the buffer by lines, not words

	studRoster.smap = make(map[string]student) //instantiate a map belonging to roster object

	// read the file and store student information
	for scanW.Scan() {
		line := strings.Fields(scanW.Text()) //removes whitespace, the need for scanWords & reads line by line
		name := line[0]                      //will be the key value in the map corresponding to id name

		claScore, _ := strconv.Atoi(line[1])  //strconv.Atoi functions similarly to C++ ATOI which converts the strings type value to an integer
		olaScore, _ := strconv.Atoi(line[2])  //since we are dealing with integer values, we can convert all data besides C# to an int
		quizScore, _ := strconv.Atoi(line[3]) //since it returns two values, we use golang built in _ to ignore the error return
		exam, _ := strconv.Atoi(line[4])
		finalExam, _ := strconv.Atoi(line[5])

		if len(name) == 8 {
			studRoster.smap[name] = student{ //define the structs field function

				finalNumberGrade: func(claScore int, olaScore int, quizScore int, exam int, finalExam int) int {
					return (claScore + olaScore + quizScore + exam + finalExam)
				},
			}

			studRoster.smap[name] = student{ //define each instance of the struct with the updated values
				id:          name,
				claScores:   claScore,
				olaScores:   olaScore,
				quizScores:  quizScore,
				exam:        exam,
				finalExam:   finalExam,
				totalPoints: studRoster.smap[name].finalNumberGrade(claScore, olaScore, quizScore, exam, finalExam),
				letterGrade: finalLetterGrade(studRoster.smap[name].finalNumberGrade(claScore, olaScore, quizScore, exam, finalExam)),
			}

			studRoster.count++ //increase roster's size
		} else { // if c-number is not the correct length, print a message
			fmt.Println("Invalid C-Number")
		}
	}

	//perform the query, after 2 user inputs, print all student information using the struct of structs
	query(studRoster)

	//terminate program after satisfying queries
	dataFile.Close() //always close an open file

}
