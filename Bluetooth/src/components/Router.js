import React from 'react';
import { Scene, Router, Actions } from 'react-native-router-flux';
import Bluetooth from './Bluetooth';
import Slider from './Slider';
import Sensors from './Sensors';
import Schedule from './Schedule';
import Weather from './Weather';

const RouterComponent = () => {
  return (
    <Router>
      <Scene key="root" hideNavBar>

        <Scene key="intial">
          <Scene key="connectivity" component={Bluetooth} title="Connect to Bluetooth" />
        </Scene>

        <Scene key='connected'>
          <Scene
          rightTitle='Sensors'
          onRight={() => Actions.sensors()}
          key="slider"
          component={Slider}
          title="Slider"
          />
          <Scene
          rightTitle='Schedule'
          onRight={() => Actions.schedule()}
           key='sensors'
           component={Sensors}
           title="Sensors"
          />
          <Scene
           key='schedule'
           component={Schedule}
           title="Scheduling"
          />

        </Scene>
      </Scene>
    </Router>
  );
};

export default RouterComponent;
