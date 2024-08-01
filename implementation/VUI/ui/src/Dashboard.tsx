import React, { useState, useEffect } from "react";
import HeaterDisplay from "./HeaterDisplay";
import DeviceStatus from "./DeviceStatus";

const Dashboard: React.FC = () => {
  const [temperatures, setTemperatures] = useState<number[]>([0, 0, 0, 0]);
  const [deviceStates, setDeviceStates] = useState<boolean[]>([false, false, false, false]);

  useEffect(() => {
    const interval = setInterval(() => {
      setTemperatures([
        Math.random() * 100,
        Math.random() * 100,
        Math.random() * 100,
        Math.random() * 100,
      ]);
      setDeviceStates([
        Math.random() > 0.5,
        Math.random() > 0.5,
        Math.random() > 0.5,
        Math.random() > 0.5,
      ]);
    }, 5000);

    return () => clearInterval(interval);
  }, []);

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
