import {useEffect, useState} from "react";
import './App.css';
import {
    mhAddParameterListener, mhRemoveParameterListener,
    mhSubscribeToParams, mhUnsubscribeFromParams
} from "@slm-audio/mostly-harmless-js/lib/mostlyharmless_ParamEvents.js";
import {
    mhBeginParamGesture,
    mhEndParamGesture,
    mhSetParamValue
} from "@slm-audio/mostly-harmless-js/lib/mostlyharmless_ParamGestures.js";

function SliderComponent({pid, min, max, initial}) {
    const [val, setVal] = useState(initial);
    const paramCallback = (ev) => {
        const value = parseFloat(ev.detail.value);
        setVal(value);
    }
    useEffect(() => {
        mhSubscribeToParams();
        mhAddParameterListener(pid, paramCallback);
        return function cleanup() {
            mhRemoveParameterListener(pid, paramCallback);
            mhUnsubscribeFromParams();
        }
    }, [paramCallback]);

    const sliderDragStarted = (v) => {
        mhBeginParamGesture(pid);
    };
    const sliderChangedCallback = (v) => {
        setVal(v.target.value);
        mhSetParamValue(pid, v.target.value);
    };

    const sliderDragEnded = (v) => {
        mhEndParamGesture(pid);
    };
   
    return (
        <div>
            <input className="slider-el"
                   type="range"
                   value={val}
                   onDragStart={sliderDragStarted}
                   onChange={sliderChangedCallback}
                   onDragEnd={sliderDragEnded}
                   min={min} max={max} step={0.0001}></input>

        </div>
    )
}

export default SliderComponent;