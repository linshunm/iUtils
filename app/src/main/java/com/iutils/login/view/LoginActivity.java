package com.iutils.login.view;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.text.InputType;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import com.iutils.common.BaseActivity;
import com.iutils.R;
import com.iutils.leak.StaticInnerClassActivity;
import com.iutils.login.presenter.LoginPresenter;
import com.iutils.main.view.MainActivity;
import com.iutils.utils.Const;
import com.iutils.utils.ILog;
import com.iutils.utils.SystemUtil;
import com.iutils.utils.ToastUtil;
import com.iutils.widgets.ObservableScrollView;
import com.iutils.widgets.ScrollViewListener;

/**
 * A login screen that offers login via email/password.
 */
public class LoginActivity extends BaseActivity implements ILoginView,
        View.OnClickListener, View.OnFocusChangeListener,
        ScrollViewListener, CompoundButton.OnCheckedChangeListener,
        ActivityCompat.OnRequestPermissionsResultCallback {

    private final static String TAG = "LoginActivity";
    private LoginPresenter presenter;
    ObservableScrollView svLayout;
    private RelativeLayout rlRoot;
    private RelativeLayout rlLogin;
    private EditText etUserId;
    private EditText etUserPwd;
    private ImageView ivLogo;
    private ImageView ivSettings;
    private ImageView ivClearUserId;
    private CheckBox ckbShowPwd;
    private Button btnLogin;
    private boolean isFinishedScrolled = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        presenter = new LoginPresenter(this);

        initWidget();

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        {
            //申请WRITE_EXTERNAL_STORAGE权限
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, Const.WRITE_EXTERNAL_STORAGE_REQUEST_CODE);
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_PHONE_STATE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_PHONE_STATE}, Const.READ_PHONE_STATE_REQUEST_CODE);
        }

    }

    /**
     * 初始化UI
     */
    private void initWidget() {
        svLayout = (ObservableScrollView) findViewById(R.id.sv_top);
        svLayout.setScrollViewListener(this);

        rlRoot = (RelativeLayout) findViewById(R.id.rl_root);
        rlLogin = (RelativeLayout) findViewById(R.id.rl_login);

        etUserId = (EditText) findViewById(R.id.et_user_id);
        etUserId.setOnClickListener(this);
        etUserId.setOnFocusChangeListener(this);

        etUserPwd = (EditText) findViewById(R.id.et_user_pwd);
        etUserPwd.setOnClickListener(this);
        etUserPwd.setOnFocusChangeListener(this);

        ivLogo = (ImageView) findViewById(R.id.iv_logo);
        ivLogo.setOnClickListener(this);
        ivSettings = (ImageView) findViewById(R.id.iv_settings);
        ivSettings.setOnClickListener(this);
        ivClearUserId = (ImageView) findViewById(R.id.iv_clear_user_id);
        ivClearUserId.setOnClickListener(this);
        ckbShowPwd = (CheckBox) findViewById(R.id.chk_show_pwd);
        ckbShowPwd.setOnCheckedChangeListener(this);
        ivLogo.setOnClickListener(this);
        btnLogin = (Button) findViewById(R.id.btn_login);
        btnLogin.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.iv_logo: {
                hideSoftInputWindow();
                break;
            }
            case R.id.iv_settings: {
                break;
            }
            case R.id.iv_clear_user_id: {
                clearUserId();
                break;
            }
            case R.id.btn_login: {
                login();
                break;
            }
            case R.id.et_user_id:
            case R.id.et_user_pwd: {
                scrollToBottom();
                break;
            }
        }

    }

    private void login()
    {
        ILog.i(TAG, "login...");
        presenter.login();
    }

    private void clearUserId()
    {
        if(etUserId != null)
        {
            etUserId.setText("");
        }
    }


    /**
     * 隐藏输入法
     */
    public void hideSoftInputWindow() {
        if(isSoftInputShow())
        {
            InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(getWindow().getDecorView().getWindowToken(), 0);
        }
    }

    @Override
    public void gotoMainActivity() {
        ILog.i(TAG, "gotoMainActivity()");
        Intent intent = new Intent(this, MainActivity.class);

        startActivity(intent);
        finish();
    }

    public void resetLoginName(String name)
    {
        btnLogin.setText(name);
    }




    @Override
    public void onFocusChange(View v, boolean hasFocus) {
        switch (v.getId()) {
            case R.id.et_user_id:
            case R.id.et_user_pwd: {
                ILog.d(TAG, "view[" + v.getId() + "] hasFocus[" + hasFocus + "]");
                if (hasFocus) {
                    scrollToBottom();
                }
                break;
            }
        }

    }

    private void scrollToBottom() {
        isFinishedScrolled = false;
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (isSoftInputShow()) {
                    Rect rect = new Rect();
                    rlRoot.getWindowVisibleDisplayFrame(rect);
                    svLayout.scrollTo(0, rect.bottom);
                    isFinishedScrolled = true;
                    ILog.d(TAG, "rect.bottom[" + rect.bottom
                            + "] getMeasuredHeight[" + svLayout.getMeasuredHeight()
                            + "] getHeight[" + svLayout.getHeight()
                            + "] getMaxScrollAmount[" + svLayout.getMaxScrollAmount() + "]");
                }


            }
        }, 300);
    }

    private Handler mHandler = new Handler() {

    };


    @Override
    public void onScrollChanged(ObservableScrollView scrollView, int x, int y, int oldx, int oldy) {
        ILog.d(TAG, "scrollView[" + scrollView.getId()
                + "] scrollY[" + y + "] oldScrollY[" + oldy
                + "]");
        if(Math.abs(y - oldy)>10 && isFinishedScrolled)
        {
            //当弹出输入法且滑动界面时，则隐藏输入法
            hideSoftInputWindow();
        }
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        switch (buttonView.getId()) {
            case R.id.chk_show_pwd: {
            //是否显示密码
            etUserPwd.setInputType(isChecked ? InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD : InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
            //编辑框中光标定位到最后
            CharSequence text = etUserPwd.getText();
                if (text != null) {
                    etUserPwd.setSelection(text.length());
                }
                break;
            }
        }
    }

    private boolean isSoftInputShow()
    {
        boolean isShow;
        Rect rect = new Rect();
        rlRoot.getWindowVisibleDisplayFrame(rect);
        int screenHeight = SystemUtil.getScreenHeight();
        int visibleHeight = rect.bottom;
        ILog.d(TAG, "isSoftInputShow screenHeight[" + screenHeight+"] visibleHeight["+visibleHeight+"]");
        if(screenHeight - visibleHeight >100)
        {
            isShow = true;
        }
        else
        {
            isShow = false;
        }


        return isShow;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        presenter.release();

        ILog.d(TAG, "onDestroy");
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode)
        {
            case Const.WRITE_EXTERNAL_STORAGE_REQUEST_CODE:
            {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // Permission Granted
                    ToastUtil.showToast("Permission Granted");
                } else {
                    // Permission Denied
                    ToastUtil.showToast("Permission Denied");
                }
                break;
            }
            case Const.READ_PHONE_STATE_REQUEST_CODE:
            {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // Permission Granted
                    ToastUtil.showToast("Permission Granted");
                } else {
                    // Permission Denied
                    ToastUtil.showToast("Permission Denied");
                }
                break;
            }
        }
    }
}

