import React, { useState, useEffect } from "react";
import HeaterDisplay from "./HeaterDisplay";
import DeviceStatus from "./DeviceStatus";

const Dashboard: React.FC = ({ LastData: LastData}) => {
  const [temperatures, setTemperatures] = useState<number[]>([0, 0, 0, 0])
  const [deviceStates, setDeviceStates] = useState<boolean[]>([false, false, false, false])

  useEffect(() => {
    if (LastData) {
      setTemperatures([LastData.THERM_01, LastData.THERM_02, LastData.THERM_03, LastData.THERM_04])
      setDeviceStates([LastData.HTR_01 === 'On', LastData.HTR_02 === 'On', LastData.HTR_03 === 'On', LastData.HTR_04 === 'On'])
    }
  }, [LastData])


  return (
    <div className="max-w-4xl mx-10 flex flex-col gap-3">
      {/* Heater Displays Row */}
      <div className="flex flex-wrap gap-4 justify-between">
        {temperatures.map((temp, index) => (
          <HeaterDisplay key={index} temperature={temp} index={index} />
        ))}
      </div>
      
      {/* Device Status Row */}
      <div className="flex flex-wrap gap-4 justify-between">
        {deviceStates.map((state, index) => (
          <DeviceStatus key={index} isOn={state} index={index} />
        ))}
      </div>
    </div>
  );
};

export default Dashboard;
