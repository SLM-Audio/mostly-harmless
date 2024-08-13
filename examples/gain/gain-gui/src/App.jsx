import { useState, useEffect } from 'react'
import './App.css'

function App() {
    const [val, setVal] = useState(0.0);
    useEffect(() => {
        window.addEventListener("param-event", (ev) => {
            setVal(parseFloat(ev.detail.value));
        });
    }, [] );

    const sliderDragStarted = (v) => {
        console.log("drag started");

        beginParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    const sliderChangedCallback = (v) => {
        setVal(v.target.value);
        const args = {
            paramId: 0,
            value: v.target.value
        };
        setParamValue(args).then((res) => console.log(res));
    };

    const sliderDragEnded = (v) => {
        endParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
  return (
    <>
      <div>
          <p className="label">Gain</p>
          <input className="slider-el"
                 type={"range"}
                 value={val}
                 onDragStart={sliderDragStarted}
                 onChange={sliderChangedCallback}
                 onDragEnd={sliderDragEnded}
                 min={0} max={1} step={0.001}></input>
      </div>
    </>
  )
}

export default App
