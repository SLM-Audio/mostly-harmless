import {useEffect, useState} from "react";
import './App.css';

function SliderComponent({pid, min, max}) {
    const [val, setVal] = useState(0.1);
    const paramCallback = (ev) => {
        const eventPid = ev.detail.paramId;
        if (parseInt(eventPid) !== pid) return;
        const value = parseFloat(ev.detail.value);
        setVal(value);
    }
    useEffect(() => {
        addEventListener("param", paramCallback);
        return function cleanup() {
            removeEventListener("param", paramCallback);
        }
    }, [paramCallback]);

    const sliderDragStarted = (v) => {
        beginParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    const sliderChangedCallback = (v) => {
        setVal(v.target.value);
        const args = {
            paramId: 0,
            value: parseFloat(v.target.value)
        };
        setParamValue(args);
    };

    const sliderDragEnded = (v) => {
        endParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    return (
        <div>
            <input className="slider-el"
                   type="range"
                   value={val}
                   onDragStart={sliderDragStarted}
                   onChange={sliderChangedCallback}
                   onDragEnd={sliderDragEnded}
                   min={0} max={1} step={0.01}></input>

        </div>
    )
}

export default SliderComponent;