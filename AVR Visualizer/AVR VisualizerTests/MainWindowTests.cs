using Microsoft.VisualStudio.TestTools.UnitTesting;
using AVR_Visualizer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AVR_Visualizer.Tests
{
    [TestClass()]
    public class MainWindowTests
    {
        [TestMethod()]
        public void ReturnStringTest()
        {
            List<int> str = new List<int>()
            {
                0xff,0xff,0xff,0xff,
                0xff,0xff,0xff,0xff,
                0xff,0xff,0xff,0xff,
                0xff
            };

            for (int i = 0; i < 8; i++)
            {
                var sstring = AvrHelper.ReturnString(str, i, 0);
                Assert.AreEqual("OUT High", sstring);
            }


        }

        [TestMethod()]
        public void ReturnStringTest2()
        {
            List<int> str = new List<int>()
            {
                0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,
                0x00
            };

            for (int i = 0; i < 8; i++)
            {
                var sstring = AvrHelper.ReturnString(str, i, 0);
                Assert.AreEqual("IN Low", sstring);

            }
        }

        [TestMethod()]
        public void ReturnStringTest3()
        {
            List<int> str = new List<int>()
            {
                0xF0,0x00,0x00,0x00,       //Pin
                0x0F,0x00,0x00,0x00,          //PORT
                0xF1,0x00,0x00,0x00,     //DDR
                0x00
            };

            var sstring = AvrHelper.ReturnString(str, 0, 0);
            Assert.AreEqual("OUT High", sstring);

            sstring = AvrHelper.ReturnString(str, 1, 0);
            Assert.AreEqual("IN Low", sstring);

            sstring = AvrHelper.ReturnString(str, 2, 0);
            Assert.AreEqual("IN Low", sstring);

            sstring = AvrHelper.ReturnString(str, 3, 0);
            Assert.AreEqual("IN Low", sstring);

            sstring = AvrHelper.ReturnString(str, 4, 0);
            Assert.AreEqual("OUT Low", sstring);

            sstring = AvrHelper.ReturnString(str, 5, 0);
            Assert.AreEqual("OUT Low", sstring);

            sstring = AvrHelper.ReturnString(str, 6, 0);
            Assert.AreEqual("OUT Low", sstring);

            sstring = AvrHelper.ReturnString(str, 7, 0);
            Assert.AreEqual("OUT Low", sstring);
        }
    }
}