public class Mouse {
   public static final int NONE_BUTTON = 0;
   public static final int LEFT_BUTTON = 1;
   public static final int MIDDLE_BUTTON = 2;
   public static final int UNKNOWN_BUTTON = 3;

   public static final int NONE_STATE = 0;
   public static final int PRESSED_STATE = 1;
   public static final int RELEASED_STATE = 2;

   public float x;
   public float y;
   public int button;
   public int button_state;
}