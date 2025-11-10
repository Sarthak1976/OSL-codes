#!/bin/bash

address_book="$HOME/Desktop/AddressBook.txt"
id=1

while true
do
    echo "-------------------"
    echo "========== MENU =========="
    echo "1. Create Address Book"
    echo "2. View Address Book"
    echo "3. Insert a record"
    echo "4. Delete a record"
    echo "5. Modify a record"
    echo "6. Exit"
    echo "-------------------"
    echo ""
    read -p "Enter your choice: " ch

    case $ch in
        1)
            if [ -f "$address_book" ]; then
                echo "Address book already created at $address_book"
            else
                touch "$address_book"
                echo "Address book successfully created at $address_book"
            fi
            ;;

        2)
            if [ -f "$address_book" ]; then
                if [ -s "$address_book" ]; then
                    echo "---------- Address Book ----------"
                    cat "$address_book"
                    echo "---------- End of Address Book ----------"
                else
                    echo "Address Book is empty!"
                fi
            else
                echo "Address Book doesn't exist! Create one first."
            fi
            ;;

        3)
            if [ -f "$address_book" ]; then
                read -p "Enter the Name to store: " name
                read -p "Enter the person's phone number: " num
                echo "$id;$name;$num" >> "$address_book"
                ((id++))
                echo "Record added successfully!"
            else
                echo "Address Book doesn't exist! Create one first."
            fi
            ;;

        4)
            if [ -f "$address_book" ]; then
                read -p "Enter the name to delete: " name

                if grep -qi ";$name;" "$address_book"; then
                    sed -i "/;$name;/Id" "$address_book"
                    echo "Record for $name deleted successfully!"
                else
                    echo "Record not found!"
                fi
            else
                echo "Address Book doesn't exist! Create one first."
            fi
            ;;

        5)
            if [ -f "$address_book" ]; then
                read -p "Enter Name whose number you want to change: " name

                if grep -qi ";$name;" "$address_book"; then
                    read -p "Enter the person's new number: " newnum
                    sed -i "s/\(.*;$name;\)[0-9]*/\1$newnum/I" "$address_book"
                    echo "Record for $name modified successfully!"
                else
                    echo "Record not found!"
                fi
            else
                echo "Address Book doesn't exist! Create one first."
            fi
            ;;

        6)
            echo "Exiting..."
            exit 0
            ;;

	7)
            if [ -f "$address_book" ]; then
                read -p "Enter the Name to display: " name

                if grep -qi ";$name;" "$address_book"; then
                    echo "---------- Record Found ----------"
                    grep -i ";$name;" "$address_book" | column -t -s ';'
                    echo "----------------------------------"
                else
                    echo "Record not found!"
                fi
            else
                echo "Address Book doesn't exist! Create one first."
            fi
            ;;


	 8)
            if [ -f "$address_book" ]; then
                if [ -s "$address_book" ]; then
                    echo "---------- Sorted by Last Name ----------"
                    awk -F';' '{print $0, $(NF-1)}' "$address_book" \
                    | sort -k3,3 \
                    | cut -d' ' -f1-1 \
                    | column -t -s ';'
                    echo "----------------------------------------"
                else
                    echo "Address Book is empty!"
                fi
            else
                echo "Address Book doesn't exist! Create one first."
            fi
            ;;


	


        *)
            echo "Invalid Choice!!"
            ;;
    esac

    echo ""
done




sort -t ';' -k2,2 "$address_book" | column -t -s ';'

