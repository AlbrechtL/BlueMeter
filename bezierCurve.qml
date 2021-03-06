/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import "../contents"
import "../../shared"

Item {
    id:container
    width: 320
    height: 480

    Column {
        spacing:5
        anchors.fill: parent
        anchors.topMargin: 12

        Text {
            font.pointSize: 24
            font.bold: true
            text: "Bezier Curve"
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#777"
        }

        Canvas {
            id:canvas
            width:320
            height:280
            property color strokeStyle:  Qt.darker(fillStyle, 1.4)
            property color fillStyle: "#b40000" // red
            property int lineWidth: lineWidthCtrl.value
            property bool fill: true
            property bool stroke: true
            property real alpha: 1.0
            property real scale : scaleCtrl.value
            property real rotate : rotateCtrl.value
            antialiasing: true

            onLineWidthChanged:requestPaint();
            onFillChanged:requestPaint();
            onStrokeChanged:requestPaint();
            onScaleChanged:requestPaint();
            onRotateChanged:requestPaint();

            onPaint: {
                var ctx = canvas.getContext('2d');
                var originX = 85
                var originY = 75


                //ctx.fill();
                ctx.save();
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                ctx.translate(originX, originX);
                ctx.globalAlpha = canvas.alpha;
                ctx.strokeStyle = canvas.strokeStyle;
                ctx.fillStyle = canvas.fillStyle;
                ctx.lineWidth = canvas.lineWidth;

                ctx.translate(originX, originY)
                ctx.scale(canvas.scale, canvas.scale);
                ctx.rotate(canvas.rotate);
                ctx.translate(-originX, -originY)

                ctx.beginPath();
                arrow(ctx, 0, 0, 200, 0, [20, 15, 20, 5, -20, 5, -20, 15]);
                ctx.closePath();


//                //! [0]
//                ctx.beginPath();
//                ctx.moveTo(75,40);
//                ctx.bezierCurveTo(75,37,70,25,50,25);
//                ctx.bezierCurveTo(20,25,20,62.5,20,62.5);
//                ctx.bezierCurveTo(20,80,40,102,75,120);
//                ctx.bezierCurveTo(110,102,130,80,130,62.5);
//                ctx.bezierCurveTo(130,62.5,130,25,100,25);
//                ctx.bezierCurveTo(85,25,75,37,75,40);
//                ctx.closePath();
                //! [0]
                if (canvas.fill)
                    ctx.fill();
                if (canvas.stroke)
                    ctx.stroke();
                ctx.restore();

                // Based on https://github.com/frogcat/canvas-arrow
                // Copyright (c) 2017 Yuzo Matsuzawa
                // MIT License
                function arrow(ctx, startX, startY, endX, endY, controlPoints) {
                    var dx = endX - startX;
                    var dy = endY - startY;
                    var len = Math.sqrt(dx * dx + dy * dy);
                    var sin = dy / len;
                    var cos = dx / len;
                    var a = [];
                    a.push(0, 0);
                    for (var i = 0; i < controlPoints.length; i += 2) {
                      var x = controlPoints[i];
                      var y = controlPoints[i + 1];
                      a.push(x < 0 ? len + x : x, y);
                    }
                    a.push(len, 0);
                    for (var i = controlPoints.length; i > 0; i -= 2) {
                      var x = controlPoints[i - 2];
                      var y = controlPoints[i - 1];
                      a.push(x < 0 ? len + x : x, -y);
                    }
                    a.push(0, 0);
                    for (var i = 0; i < a.length; i += 2) {
                      var x = a[i] * cos - a[i + 1] * sin + startX;
                      var y = a[i] * sin + a[i + 1] * cos + startY;
                      if (i === 0) ctx.moveTo(x, y);
                      else ctx.lineTo(x, y);
                    }
                };
            }
        }
    }
    Column {
        id: controls
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
        Slider {id: lineWidthCtrl; min: 1; max: 10; init: 2; name: "Outline"}
        Slider {id: scaleCtrl; min: 0.1; max: 10; init: 1; name: "Scale"}
        Slider {id: rotateCtrl; min: 0; max: Math.PI*2; init: 0; name: "Rotate"}
    }
}
