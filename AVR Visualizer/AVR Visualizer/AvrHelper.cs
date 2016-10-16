using System.Collections.Generic;

namespace AVR_Visualizer
{
    public static class AvrHelper
    {
        public static string ReturnString(List<int> integers, int pinNo, int PortNo)
        {
            if (CheckDDR(integers, pinNo, PortNo))
            {
                return CheckPORT(integers, pinNo, PortNo)
                    ? "OUT High"
                    : "OUT Low";
            }
            return CheckPIN(integers, pinNo, PortNo)
                ? "IN High"
                : "IN Low";
            
        }

        public static bool CheckPORT(List<int> integers, int pinNo, int PortNo)
        {
            var masked = integers[4 + PortNo] & (1 << pinNo);

            return masked == (1 << pinNo);
        }

        public static bool CheckPIN(List<int> integers, int pinNo, int PortNo)
        {

            var masked = integers[0 + PortNo] & (1 << pinNo);
            return masked == (1 << pinNo);
        }

        public static bool CheckDDR(List<int> integers, int pinNo, int PortNo)
        {
            var masked = integers[8 + PortNo] & (1 << pinNo) ;
            return masked == (1<< pinNo);
        }
    }
}