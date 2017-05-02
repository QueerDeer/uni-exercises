import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    id: calendar
    color: "grey"
    width: 420;
    height: 420

    property date show_date: new Date()
    property date now_is: new Date()
    property int day_one: new Date(show_date.getFullYear(), show_date.getMonth(), 1).getDay()
    property int days_amount: new Date(show_date.getFullYear(), show_date.getMonth() + 1, 0).getDate()

    function isToday(index)
    {
        if (now_is.getFullYear() != show_date.getFullYear())
        {
            return false;
        }

        if (now_is.getMonth() != show_date.getMonth())
        {
            return false;
        }

        return (index == now_is.getDate() - 1)
    }

    Item
    {
        id: header
        anchors.top: parent.top
        width: parent.width
        height: childrenRect.height

        Image
        {
            source: "./left.png"
            scale: 0.42
            anchors.left: parent.left

            MouseArea
            {
                onClicked: show_date = new Date(show_date.getFullYear(), show_date.getMonth(), 0)
                anchors.fill: parent
            }
        }

        Image
        {
            source: "./right.png"
            scale: 0.42
            anchors.right: parent.right

            MouseArea {
                onClicked: show_date = new Date(show_date.getFullYear(), show_date.getMonth() + 1, 1)
                anchors.fill: parent
            }
        }

        Text
        {

            color: "black"
            text: Qt.formatDateTime(show_date, "MMMM yyyy")
            font.bold: true
            font.pixelSize: 21
            anchors.horizontalCenter: parent.horizontalCenter
        }

    }

    Item
    {
        id: body
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: calendar.height - header.height

        Item
        {
            id: days
            width: parent.width
            height: childrenRect.height

            Grid
            {
                columns: 7
                spacing: 10

                Repeater
                {
                    model: 7

                    Rectangle
                    {
                        color: "grey"
                        width: (calendar.width - 50)/7
                        height: childrenRect.height

                        Text
                        {
                            color: "black"
                            text: Qt.formatDate(new Date(show_date.getFullYear(), show_date.getMonth(), index - day_one + 1), "ddd")
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                    }

                }

            }

        }

        Item
        {
            id: dates
            width: parent.width
            anchors.top: days.bottom
            anchors.bottom: parent.bottom

            Grid
            {
                columns: 7
                rows: 6
                spacing: 10

                Repeater
                {
                    model: day_one + days_amount

                    Rectangle
                    {
                        color:
                        {
                            if (index < day_one)
                            {
                                calendar.color;
                            }
                            else
                            {
                                isToday(index - day_one) ? "lightgrey" : "grey";
                            }

                        }
                        width: (calendar.width - 50)/7
                        height: (dates.height - 50)/6

                        Text
                        {
                            text: index + 1 - day_one
                            opacity: (index < day_one) ? 0 : 1
                            font.bold: isToday(index - day_one)
                            anchors.centerIn: parent
                        }

                    }

                }

            }

        }

    }


}
